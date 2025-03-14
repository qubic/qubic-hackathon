using namespace QPI;

//
// Various tier levels and their respective constants for checks below.
//
enum tierLevel {
    NOST_NONE = 0,
    NOST_EGG = 1,
    NOST_DOG = 2,
    NOST_ALIEN = 3,
    NOST_WARRIOR = 4,
    NOST_QUEEN = 5
};

//
// Various project states and their respective constancts.
//
enum projectState {
    NOST_VOTE_STATE = 0,
    NOST_REGISTER_STATE = 1,
    NOST_INVESTMENT_PHASE_1 = 2,
    NOST_INVESTMENT_PHASE_2 = 3,
    NOST_INVESTMENT_PHASE_3 = 4,
    NOST_CLOSED_FAILED = 5,
    NOST_CLOSED_SUCCESS = 6,
    NOST_BLOCKED = 7,
    NOST_ASK_MORE_INFORMATION = 8,
    NOST_PREINVEST_STATE = 9,
    NOST_PREPARE_VOTE = 10,
    NOST_FUNDED = 11,
    NOST_DRAFT = 12,
};

//
// Constants for sizing
//
constexpr uint64 NOSTROMO_MAX_USERS = 131072;
constexpr uint64 NOSTROMO_MAX_PROJECTS = 1024;
constexpr uint64 NOSTROMO_MAX_LEVELS = 8;

//
// Return codes enum
//
enum returnCodeNost {
    NOST_SUCCESS = 0,
    NOST_INVALID_TIER = 1,
    NOST_INSUFFICIENT_BALANCE = 2,
    NOST_TIER_ALREADY_SET = 3,
    NOST_USER_NOT_FOUND = 4,
    NOST_NO_TIER_FOUND = 5,
    NOST_UNABLE_TO_UNSTAKE = 6,
    NOST_PROJECT_NOT_FOUND = 7,
    NOST_PROJECT_CREATE_FAILED = 8,
    NOST_INVALID_PROJECT_ID = 9,
    NOST_INVALID_STATE = 10,
    NOST_INVALID_TRANSITION = 11,
    NOST_ALREADY_REGISTERED = 12,
    NOST_NOT_REGISTERED = 13,
    NOST_ALREADY_VOTED = 14,
    NOST_REQUIRES_ADMIN = 15
};

//
// Vote enums for consistency in code
//
enum voteValue {
    NO_VOTE = 0,
    YES_VOTE = 1
};

struct NOST2
{
};

struct NOST : public ContractBase
{

public:

    //
    // Structure for tier definitions.
    //
    struct nostromoTier {
        uint8 stakeAmount;
        float poolWeight;
    };

    //
    // Structure to hold a projects metadata as defined below:
    //
    // owner : wallet address of project creator/owner
    // projectSt : current state of the project based on phase
    // yesvotes : number of yes votes received during voting cycle
    // novotes : number of no votes received during voting cycle
    // investOne : number of epochs spent during Investment phase 1 
    // investTwo : number of epochs spent during Investment phase 2
    // investThree : number of epochs spent during Investment phase 3
    //
    struct projectMeta {
        id owner;
        projectState projectSt;
        uint64 yesvotes;
        uint64 novotes;
        uint8 investOne;
        uint8 investTwo;
        uint8 investThree;
    };

    //
    // Structure used to hold financial data related to a project:
    //
    // totalAmount : total amount to be raised for project
    // threshold : decimal representing % of threshold between min and max caps 
    // tokenPrice : initial token price set in createProject method
    // raisedAmount : raised amount in dollars
    // raiseInQubics : number of Qubics allocated during project raise
    // tokensInSale : number of tokens allocated during initial sale
    //
    struct projectFinance {
        double totalAmount;
        float threshold;
        uint64 tokenPrice;
        uint64 raisedAmount;
        uint64 raiseInQubics;
        uint64 tokensInSale;
    };
    
    //
    // Used to keep track of project min & max caps for investment levels.
    //
    struct projectCapPairs {
        double minCap;
        double maxCap;
    };

private:

    //
    // Used to track the caps for each project.  Caps are calculated as follows:
    // Target amount to raise - threshold = min cap
    // Target amount to raise + threshold = max cap
    //
    array<projectCapPairs, NOSTROMO_MAX_PROJECTS> capTracker;

    //
    // State hash map to manage tier information
    //
    QPI::HashMap<tierLevel, nostromoTier, NOSTROMO_MAX_LEVELS> tiers;

    //
    // State hash map to manage user to tier mapping
    //
    QPI::HashMap<id, tierLevel, NOSTROMO_MAX_USERS> userTiers;

    //
    // State variable to track total QUBIC staked
    //
    uint64 stakedQubicsInContract;

    //
    // Cost of a transaction with Nostromo
    //
    sint64 transactionFee;

    //
    // Fee associated with creating a project
    //
    sint64 projectFee;    

    //
    // Counter/ID used to track projects
    //
    uint64 projectNextId;
    
    //
    // The wallet of the admin
    //
    id admin;

    //
    // Array used to hold the metadata of all projects array index corresponds to projectId
    //
    array<projectMeta,NOSTROMO_MAX_PROJECTS> projectMetadataList;

    //
    // Array used to hold the financial data of all projects array index corresponds to projectId
    //
    array<projectFinance,NOSTROMO_MAX_PROJECTS> projectFinanceList;

    //
    // Type used for user tracking.  Two lists get created, one list for vote flagging, the other for
    // investment flagging. 0 == NO & 1 == YES
    //
    typedef array<bit, NOSTROMO_MAX_PROJECTS> flags;

    //
    // Type used for tracking each user's investments.  The array represents an investment pattern per
    // user.  For example if a user invests in a project with projectId of 9 then we would set the value
    // at index 9 to be their investment amount.  This allows us to track a user's investments across all
    // projects.
    //
    typedef array<float, NOSTROMO_MAX_PROJECTS> investments;

    //
    // HashMap indexed by wallet id.  Each id is mapped to an array that maps to max number of possible projects.
    // If within the array a flag is toggled to 1 that means user has voted.  For example:
    // flags[0] ... flags[1024] if a user votes for projectId 7 then the following occurs:
    // flags[7] = 1;
    // This indicates a vote has been cast by the user with a particular ID for projectId 7. 
    //
    QPI::HashMap<id, flags, NOSTROMO_MAX_USERS> voteTracking;

    //
    // HashMap indexed by wallet id.  Each id is mapped to an array that maps to max number of possible projects.
    // If within the array a flag is toggled to 1 that means user has registered.  For example:
    // flags[0] ... flags[1024] if a user registers for projectId 7 then the following occurs:
    // flags[7] = 1;
    // In the event a user unregisters then the flag is toggled back to a 0.  For example:
    // flags[7] = 0;
    //
    QPI::HashMap<id, flags, NOSTROMO_MAX_USERS> regTracking;

    //
    // Hashmap indexed by wallet id.  Each id is mapped to an array representing that users investment 
    // activity for example a user invests in a projects with project IDs of 4, 9 and 25 we would set
    // the values appropriately for that in their investment array and then associate it to their id
    // in this HashMap.
    //
    QPI::HashMap<id, investments, NOSTROMO_MAX_USERS> investTracking;

    //
    // Epoch state variables for tracking, this will be used to determine how many epochs each
    // phase will last, can be modified by admin only.
    //
    uint8 investPhaseOneEpochs;
    uint8 investPhaseTwoEpochs;
    uint8 investPhaseThreeEpochs;

    //
    // Structures and method for calculating perUse.
    //
    // This method will walk through the registration list and for any user
    // registered the following will occur:
    //
    // 1. User's tier is extracted
    // 2. The pool weight is determined for each and a running total is created
    //
    // The output of the procedure will be used to calculate investment per tier.
    //
    struct calculatePerUse_input {
        uint64 projectId;
    };

    struct calculatePerUse_output {
        returnCodeNost status;
        double totalPerUse;
    };

    struct calculatePerUse_locals {
        uint64 index;
        flags userReg;
        id wallet;
        tierLevel tier;
        double perUse;
        nostromoTier usersTier;
    };

    PRIVATE_PROCEDURE_WITH_LOCALS(calculatePerUse)

        //
        // Make sure the ID is at least within range of what has been stored thus far
        //        
        if (state.projectNextId <= input.projectId) {
            output.status = returnCodeNost::NOST_INVALID_PROJECT_ID;
            qpi.transfer(qpi.invocator(), qpi.invocationReward());
            output.totalPerUse = 0.0;
            return;
        }
        
        //
        // Traverse the Hashmap and check each users registration status for investment.  If
        // they are registered to invest we need to grab their tier and tabulate the per use value for
        // the project.
        //
        locals.perUse = 0.0;

        for(locals.index = 0; locals.index < state.regTracking.capacity(); locals.index++) {
            locals.userReg = state.regTracking.value(locals.index);

            if(locals.userReg.get(input.projectId) == 1) {
                locals.wallet = state.regTracking.key(locals.index);
                state.userTiers.get(locals.wallet, locals.tier);
                state.tiers.get(locals.tier, locals.usersTier);
                locals.perUse += locals.usersTier.stakeAmount;
            }
        }

        output.totalPerUse = locals.perUse;
        output.status = returnCodeNost::NOST_SUCCESS;
        return;
    _

    //
    // Structures and methods for setting max/min cap for a given project.
    //
    // The cap is determined by utilizing the threshold for a project.  For example:
    //
    // A project has a fund raise goal of $250K with a threshold of 10%
    //
    // Min cap becomes 250,000 * .90 or 225,000
    // Max cap becomes 250,000 * 1.10 or 275,000
    //
    // Extracted pairs are placed in an array mapped to a particular projectID
    //
    struct calculateCaps_input {
        uint64 projectId;
    };

    struct calculateCaps_output {
        projectCapPairs projectCaps;
        returnCodeNost status;
    };

    struct calculateCaps_locals {
        projectFinance financials;
        float threshold;
        double totalAmount;
    };

    PRIVATE_PROCEDURE_WITH_LOCALS(calculateCaps)

        //
        // Make sure the ID is at least within range of what has been stored thus far
        //        
        if (state.projectNextId <= input.projectId) {
            output.status = returnCodeNost::NOST_INVALID_PROJECT_ID;
            output.projectCaps.minCap = 0.0;
            output.projectCaps.maxCap = 0.0;
            return;
        }
        
        //
        // We have identified that the projectId is valid, pull the necessary financial/metadata
        // and calculate the caps and add them to the array for tracking based on projectId.
        //
        locals.financials = state.projectFinanceList.get(input.projectId);
        output.projectCaps.minCap = (1.0 - locals.financials.threshold) * locals.financials.totalAmount;
        output.projectCaps.maxCap = (1.0 + locals.financials.threshold) * locals.financials.totalAmount;            
        output.status = returnCodeNost::NOST_SUCCESS;
    _

protected:

    //
    // Structs and method for adding a tier to a user.
    //
    struct addUserTier_input {
        tierLevel tier;
    };

    struct addUserTier_output {
        returnCodeNost status;
    };

    struct addUserTier_locals {
        tierLevel foundTier;
        nostromoTier stakingTier;
    };

    PUBLIC_PROCEDURE_WITH_LOCALS(addUserTier)
 
        //
        // Ensure proper balance transfer attempted or return
        // an error code.
        //
        if (qpi.invocationReward() < state.transactionFee) {
            output.status = returnCodeNost::NOST_INSUFFICIENT_BALANCE;
            qpi.transfer(qpi.invocator(), qpi.invocationReward());     
            return;
        }

        //
        // We must check to ensure user has the proper balance or return error code. 
        //
        if (state.tiers.get(input.tier, locals.stakingTier)) {
            if(locals.stakingTier.stakeAmount + state.transactionFee != qpi.invocationReward()) {
                output.status = returnCodeNost::NOST_INSUFFICIENT_BALANCE;
                qpi.transfer(qpi.invocator(), qpi.invocationReward());
                return;
            }
        }

        //
        // If user appears in the map chck to see if he has a tier assigned.
        // If it is NONE add new tier, if not we cannot assign the new tier.
        //
        if (state.userTiers.get(qpi.invocator(), locals.foundTier)) {
            if(locals.foundTier != tierLevel::NOST_NONE) {
                output.status = returnCodeNost::NOST_TIER_ALREADY_SET;
                qpi.transfer(qpi.invocator(), qpi.invocationReward());
                return;
            }
        }

        //
        // Set the user tier and update staking volume
        //
        state.userTiers.set(qpi.invocator(), input.tier);
        state.stakedQubicsInContract += locals.stakingTier.stakeAmount;

        //
        // Zero for status means life is good.
        //
        output.status = returnCodeNost::NOST_SUCCESS; 
        return;
    _

    //
    // Structures for and method for removing a tier for a user.
    //
    struct removeUserTier_input {

    };

    struct removeUserTier_output {
        returnCodeNost status;
    };

    struct removeUserTier_locals {
        tierLevel foundTier;
        nostromoTier stakingTier;
    };    

    PUBLIC_PROCEDURE_WITH_LOCALS(removeUserTier)
        
        // 
        // Check to ensure user has sufficient balance.
        //
        if (qpi.invocationReward() < state.transactionFee) {
            output.status = returnCodeNost::NOST_INSUFFICIENT_BALANCE;
            qpi.transfer(qpi.invocator(), qpi.invocationReward());
            return;
        }

        //
        // Validate if the user is already in a tier.
        //
        if (state.userTiers.get(qpi.invocator(), locals.foundTier)){
            if(locals.foundTier == tierLevel::NOST_NONE) {
                output.status = returnCodeNost::NOST_NO_TIER_FOUND;
                qpi.transfer(qpi.invocator(), qpi.invocationReward());
                return;
            }   
        }
        else {
            output.status = returnCodeNost::NOST_USER_NOT_FOUND;
            qpi.transfer(qpi.invocator(), qpi.invocationReward());
            return;
        }

        //
        // Set user tier to NONE
        //
        state.userTiers.set(qpi.invocator(), tierLevel::NOST_NONE);

        //
        // Return the staked qubics
        //
        state.tiers.get(locals.foundTier, locals.stakingTier);
        qpi.transfer(qpi.invocator(), locals.stakingTier.stakeAmount);

        // Update the staked qubics amount
        state.stakedQubicsInContract -= locals.stakingTier.stakeAmount;

        output.status = returnCodeNost::NOST_SUCCESS;
        return;
    _

    //
    // Structures for and method for creating a project.
    //
    struct createProject_input {
        projectFinance financeInput;
    };

    struct createProject_output {
        returnCodeNost status;
        uint64 prodId;
    };    

    struct createProject_locals {
        projectMeta metadata;
        projectFinance financials;
    };

    PUBLIC_PROCEDURE_WITH_LOCALS(createProject)

        //
        // Ensure user has proper funds for project creation
        //
        if (qpi.invocationReward() < (state.transactionFee + state.projectFee)) {
            output.status = returnCodeNost::NOST_INSUFFICIENT_BALANCE;
            qpi.transfer(qpi.invocator(), qpi.invocationReward());
            return;
        }

        //
        // Setup local structures and store them in the maintenance arrays
        //
        locals.metadata.owner = qpi.invocator();
        locals.metadata.projectSt = projectState::NOST_DRAFT;
        locals.metadata.yesvotes = 0;
        locals.metadata.novotes = 0;
        locals.metadata.investOne = 0;
        locals.metadata.investTwo = 0;
        locals.metadata.investThree = 0;

        locals.financials.totalAmount = input.financeInput.totalAmount;
        locals.financials.threshold = input.financeInput.threshold;
        locals.financials.tokenPrice = input.financeInput.tokenPrice;
        locals.financials.raisedAmount = input.financeInput.raisedAmount;
        locals.financials.raiseInQubics = input.financeInput.raiseInQubics;
        locals.financials.tokensInSale = input.financeInput.tokensInSale;
 
        //
        // Add project info to each respective mgmt array.
        //
        state.projectFinanceList.set(state.projectNextId, locals.financials);
        state.projectMetadataList.set(state.projectNextId, locals.metadata);

        //
        // Incremenet ProjectId counter and return related output data 
        //
        output.prodId = state.projectNextId;
        state.projectNextId += 1;
        output.status = returnCodeNost::NOST_SUCCESS;   
    _ 

    //
    // Structures for and method for getting a project.
    //
    struct getProject_input {
        uint64 projectIdentity;
    };

    struct getProject_output {
        returnCodeNost status;
        projectMeta metadata;
        projectFinance finance;
    };

    PUBLIC_PROCEDURE(getProject)

        //
        // Make sure the ID is at least within range of what has been stored thus far
        //
        if (input.projectIdentity >= state.projectNextId) {
            output.status = returnCodeNost::NOST_INVALID_PROJECT_ID;
            qpi.transfer(qpi.invocator(), qpi.invocationReward());
            return;
        }

        output.metadata = state.projectMetadataList.get(input.projectIdentity);
        output.finance = state.projectFinanceList.get(input.projectIdentity);
        output.status = returnCodeNost::NOST_SUCCESS;
        return;
    _

    //
    // Structures for and method for changing a project state.
    //
    struct changeProjectState_input {
        uint64 projectIdentity;
        projectState newProjectState;
    };

    struct changeProjectState_output {
        returnCodeNost status;
    };

    struct changeProjectState_locals {
        projectMeta localMeta;
        uint8 result;
    };

    PUBLIC_PROCEDURE_WITH_LOCALS(changeProjectState)

        //
        // Only admin can advance a project state or it is done implicitly by contract
        //
        if (qpi.invocator() != state.admin) {
            output.status = returnCodeNost::NOST_REQUIRES_ADMIN;
            return;
        }

        //
        // Make sure the ID is at least within range of what has been stored thus far
        //        
        if (state.projectNextId <= input.projectIdentity) {
            output.status = returnCodeNost::NOST_INVALID_PROJECT_ID;
            qpi.transfer(qpi.invocator(), qpi.invocationReward());
            return;
        }

        //
        // We should allow the state change if are in Draft or Ask for more information as the project is being promoted to next step,
        // we update the project metadata and exit method, else this is an invalid state transition.
        if (input.newProjectState == projectState::NOST_PREPARE_VOTE) {
            locals.localMeta = state.projectMetadataList.get(input.projectIdentity);
            if (locals.localMeta.projectSt == projectState::NOST_DRAFT || locals.localMeta.projectSt == projectState::NOST_ASK_MORE_INFORMATION) {
                locals.localMeta.projectSt = projectState::NOST_PREPARE_VOTE;
                state.projectMetadataList.set(input.projectIdentity, locals.localMeta);
                output.status = returnCodeNost::NOST_SUCCESS;
                return;
            }
            else {
                qpi.transfer(qpi.invocator(), qpi.invocationReward());
                output.status = returnCodeNost::NOST_INVALID_TRANSITION;
                return;
            }
        }

        //
        // A transition to asking for more info can only come from a draft or blocked state.  We cannot
        // request this while in the vote or investment stages.
        //
        if (input.newProjectState == projectState::NOST_ASK_MORE_INFORMATION) {
            locals.localMeta = state.projectMetadataList.get(input.projectIdentity);
            if (locals.localMeta.projectSt == projectState::NOST_DRAFT || locals.localMeta.projectSt == projectState::NOST_BLOCKED) {
                locals.localMeta.projectSt = projectState::NOST_DRAFT;
                state.projectMetadataList.set(input.projectIdentity, locals.localMeta);
                output.status = returnCodeNost::NOST_SUCCESS;
                return;                
            } 
            else {
                qpi.transfer(qpi.invocator(), qpi.invocationReward());
                output.status = returnCodeNost::NOST_INVALID_TRANSITION;
                return;
            }
        }

        //
        // If we get this far an attempt to transition to a state isn't allowed
        // based on contract rules.  Other states should require automated SC
        // actions to ensure movement forward.
        //
        output.status = returnCodeNost::NOST_INVALID_TRANSITION;
        return;
    _

    //
    // Structures and method used to register for a project.
    //
    struct regForProject_input {
        uint64 projectIdentity;
    };

    struct regForProject_output {
        returnCodeNost status;
    };

    struct regForProject_locals {
        flags userFlags; 
        projectMeta metadata;
    };

    PUBLIC_PROCEDURE_WITH_LOCALS(regForProject)

        //
        // Ensure proper balance transfer attempted or return
        // an error code.
        //
        if (qpi.invocationReward() < state.transactionFee) {
            output.status = returnCodeNost::NOST_INSUFFICIENT_BALANCE;
            qpi.transfer(qpi.invocator(), qpi.invocationReward());     
            return;
        }  
    
        //
        // Check is project ID is valid
        //
        if (input.projectIdentity >= state.projectNextId) {
            output.status = returnCodeNost::NOST_INVALID_PROJECT_ID;
            qpi.transfer(qpi.invocator(), qpi.invocationReward());
            return;
        }

        //
        // Make sure we are in the correct state
        //
        locals.metadata = state.projectMetadataList.get(input.projectIdentity);
    
        if (locals.metadata.projectSt == projectState::NOST_REGISTER_STATE) {
            //
            // Check to see if user is in the registration list, if so are they
            // registered yet?  If they aren't then add them and setup the registration.
            //
            if(state.regTracking.get(qpi.invocator(), locals.userFlags)) {
                if (locals.userFlags.get(input.projectIdentity) == 1) {
                    output.status = returnCodeNost::NOST_ALREADY_REGISTERED;
                    qpi.transfer(qpi.invocator(), qpi.invocationReward());
                    return;
                }
                else {
                    locals.userFlags.set(input.projectIdentity, 1);
                    state.regTracking.set(qpi.invocator(), locals.userFlags);
                    output.status = returnCodeNost::NOST_SUCCESS;
                    return;
                }
            }
            else {
                locals.userFlags.set(input.projectIdentity, 1);
                state.regTracking.set(qpi.invocator(), locals.userFlags);
                output.status = returnCodeNost::NOST_SUCCESS;
                return;
            }
        }
        else {
            output.status = returnCodeNost::NOST_INVALID_STATE;
            qpi.transfer(qpi.invocator(), qpi.invocationReward());
            return;
        }
    _

    //
    // Structures and method used to unregister for a project.
    //
    struct unregForProject_input {
        uint64 projectIdentity;
    };

    struct unregForProject_output {
        returnCodeNost status;
    };

    struct unregForProject_locals {
        flags userFlags; 
        projectMeta metadata;
    };

    PUBLIC_PROCEDURE_WITH_LOCALS(unregForProject)

        //
        // Ensure proper balance transfer attempted or return
        // an error code.
        //
        if (qpi.invocationReward() < state.transactionFee) {
            output.status = returnCodeNost::NOST_INSUFFICIENT_BALANCE;
            qpi.transfer(qpi.invocator(), qpi.invocationReward());     
            return;
        }

        //
        // Check is project ID is valid
        //
        if (input.projectIdentity >= state.projectNextId) {
            output.status = returnCodeNost::NOST_INVALID_PROJECT_ID;
            qpi.transfer(qpi.invocator(), qpi.invocationReward());
            return;
        }
        
        //
        // Make sure we are in the correct state
        //
        locals.metadata = state.projectMetadataList.get(input.projectIdentity);
    
        if (locals.metadata.projectSt == projectState::NOST_REGISTER_STATE) {
            //
            // Check to see if user is in the registration list, if so are they
            // registered yet?  If they aren't indicate operation is not necessary
            // otherwise set the flag appropriately and move on.
            //
            if(state.regTracking.get(qpi.invocator(), locals.userFlags)) {
                if (locals.userFlags.get(input.projectIdentity) == 0) {
                    output.status = returnCodeNost::NOST_NOT_REGISTERED;
                    qpi.transfer(qpi.invocator(), qpi.invocationReward());
                    return;
                }
                else {
                    locals.userFlags.set(input.projectIdentity, 0);
                    state.regTracking.set(qpi.invocator(), locals.userFlags);
                    output.status = returnCodeNost::NOST_SUCCESS;
                    return;
                }
            }
            else {
                locals.userFlags.set(input.projectIdentity, 0);
                state.regTracking.set(qpi.invocator(), locals.userFlags);
                qpi.transfer(qpi.invocator(), qpi.invocationReward());
                output.status = returnCodeNost::NOST_NOT_REGISTERED;
                return;
            }            
        }
        else {
            output.status = returnCodeNost::NOST_INVALID_STATE;
            qpi.transfer(qpi.invocator(), qpi.invocationReward());
            return;
        }

    _

    //
    // Structures and method used to vote for a project.
    //
    struct voteProject_input {
        uint64 projectId;
        voteValue vote;
    };

    struct voteProject_output {
        returnCodeNost status;
    };

    struct voteProject_locals {
        tierLevel localTier;
        projectMeta metadata;
        flags votingList;

    };

    PUBLIC_PROCEDURE_WITH_LOCALS(voteProject)

        //
        // Ensure proper balance transfer attempted or return
        // an error code.
        //
        if (qpi.invocationReward() < state.transactionFee) {
            output.status = returnCodeNost::NOST_INSUFFICIENT_BALANCE;
            qpi.transfer(qpi.invocator(), qpi.invocationReward());     
            return;
        }
        
        //
        // Make sure its a valid project
        //
        if (input.projectId >= state.projectNextId) {
            output.status = returnCodeNost::NOST_INVALID_PROJECT_ID;
            qpi.transfer(qpi.invocator(), qpi.invocationReward());
            return;            
        }        

        //
        // Make sure the user is listed has a vote tier.  If we pass those 
        // conditions we carry on. 
        //   
        if (state.userTiers.get(qpi.invocator(), locals.localTier)) {
            if (locals.localTier == tierLevel::NOST_NONE) {
                output.status = returnCodeNost::NOST_INVALID_TIER;
                qpi.transfer(qpi.invocator(), qpi.invocationReward());
                return;            
            }       
            else {
                //
                // Get the project metadata and make sure we are in the vote state
                //
                locals.metadata = state.projectMetadataList.get(input.projectId);

                if (locals.metadata.projectSt != projectState::NOST_VOTE_STATE) {
                    output.status = returnCodeNost::NOST_INVALID_STATE;
                    qpi.transfer(qpi.invocator(), qpi.invocationReward());
                    return;
                }
                else {
                    //
                    // Check to see if user has voted, if so do nothing otherwise process
                    // the vote and increment the appropriate counter.
                    //
                    if (state.voteTracking.get(qpi.invocator(),locals.votingList)) {
                        if (locals.votingList.get(input.projectId) == 0) {
                            locals.votingList.set(input.projectId, 1);

                            //
                            // Apply vote to appropriate counter.
                            //
                            if (input.vote == voteValue::NO_VOTE) {
                                locals.metadata.novotes += 1;
                            }
                            else {
                                locals.metadata.yesvotes += 1;
                            }

                            //
                            // Assign vote list and metadata update to state variables.
                            //
                            state.voteTracking.set(qpi.invocator(), locals.votingList);
                            state.projectMetadataList.set(input.projectId, locals.metadata);
                            output.status = returnCodeNost::NOST_SUCCESS;
                            return;
                        }
                        else {
                            output.status = returnCodeNost::NOST_ALREADY_VOTED;
                            qpi.transfer(qpi.invocator(), qpi.invocationReward());
                            return;
                        }
                    }
                    else {
                        output.status = returnCodeNost::NOST_INVALID_TIER;
                        qpi.transfer(qpi.invocator(), qpi.invocationReward());
                        return;                         
                    }
                }
            }     
        }
        else {
            output.status = returnCodeNost::NOST_INVALID_TIER;
            qpi.transfer(qpi.invocator(), qpi.invocationReward());
            return;            
        }

    _

    //
    // Structures and method used to check on a projects vote.
    //
    struct checkProjectVote_input {
        uint64 projectId;
    };

    struct checkProjectVote_output {
        uint64 yesvotes;
        uint64 novotes;
        returnCodeNost status;
    };

    struct checkProjectVote_locals {
        projectMeta metadata;
    };
    
    PUBLIC_PROCEDURE_WITH_LOCALS(checkProjectVote)

        //
        // Just make sure the ProjectId is valid and grab the respective vote
        // counts for yes and no.
        //
        if (input.projectId < state.projectNextId) {
            locals.metadata = state.projectMetadataList.get(input.projectId);
            output.yesvotes = locals.metadata.yesvotes;
            output.novotes = locals.metadata.novotes;
            output.status = returnCodeNost::NOST_SUCCESS;
        }
        else {
            output.status = returnCodeNost::NOST_INVALID_PROJECT_ID;
        }
        return;
    _

    //
    // Methods and structures used by an admin to set the epoch counts
    // for each investment phase.  
    // 
    struct setPhaseOneEpochs_input {
        uint8 epochs;
    };

    struct setPhaseOneEpochs_output {
        returnCodeNost status;
    };

    struct setPhaseOneEpochs_locals {
        bit result;
    };

    PUBLIC_PROCEDURE_WITH_LOCALS(setPhaseOneEpochs)

        //
        // Perform admin check
        //
        if (qpi.invocator() != state.admin) {
            output.status = returnCodeNost::NOST_REQUIRES_ADMIN;
            return;
        }

        state.investPhaseOneEpochs = input.epochs;
        output.status = returnCodeNost::NOST_SUCCESS;
        return;
    _

    struct setPhaseTwoEpochs_input {
        uint8 epochs;
    };

    struct setPhaseTwoEpochs_output {
        returnCodeNost status;
    };

    struct setPhaseTwoEpochs_locals {
        bit result;
    };    

    PUBLIC_PROCEDURE_WITH_LOCALS(setPhaseTwoEpochs)

        //
        // Perform admin check
        //
        if (qpi.invocator() != state.admin) {
            output.status = returnCodeNost::NOST_REQUIRES_ADMIN;
            return;
        }

        state.investPhaseTwoEpochs = input.epochs;
        output.status = returnCodeNost::NOST_SUCCESS;
        return;
    _

    struct setPhaseThreeEpochs_input {
        uint8 epochs;
    };

    struct setPhaseThreeEpochs_output {
        returnCodeNost status;
    };

    struct setPhaseThreeEpochs_locals {
        bit result;
    };

    PUBLIC_PROCEDURE_WITH_LOCALS(setPhaseThreeEpochs)

        //
        // Perform admin check
        //
        if (qpi.invocator() != state.admin) {
            output.status = returnCodeNost::NOST_REQUIRES_ADMIN;
            return;
        }

        state.investPhaseThreeEpochs = input.epochs;
        output.status = returnCodeNost::NOST_SUCCESS;
        return;
    _    

    //
    // Structures and methods used to invest in a project.
    //
    struct investInProject_input {
        uint64 projectId;
        uint64 investmentAmount;
    };

    struct investInProject_output {

    };

    struct investInProject_locals {

    };

    PUBLIC_PROCEDURE_WITH_LOCALS(investInProject)


    _

    REGISTER_USER_FUNCTIONS_AND_PROCEDURES

        REGISTER_USER_PROCEDURE(addUserTier, 1);
        REGISTER_USER_PROCEDURE(removeUserTier, 2);
        REGISTER_USER_PROCEDURE(createProject, 3);
        REGISTER_USER_PROCEDURE(getProject, 4);
        REGISTER_USER_PROCEDURE(changeProjectState, 5);
        REGISTER_USER_PROCEDURE(regForProject, 6);
        REGISTER_USER_PROCEDURE(unregForProject, 7);
        REGISTER_USER_PROCEDURE(voteProject, 8);
        REGISTER_USER_PROCEDURE(checkProjectVote, 9);
        REGISTER_USER_PROCEDURE(setPhaseOneEpochs, 10);
        REGISTER_USER_PROCEDURE(setPhaseTwoEpochs, 11);
        REGISTER_USER_PROCEDURE(setPhaseThreeEpochs, 12);            

    _

    INITIALIZE

        //
        // Initialize the tier information.
        //
        state.tiers.reset();
        state.tiers.set(tierLevel::NOST_EGG, nostromoTier{ 1, 5.5 });
        state.tiers.set(tierLevel::NOST_DOG, nostromoTier{ 5, 30.0 });
        state.tiers.set(tierLevel::NOST_ALIEN, nostromoTier{ 10, 75.0 });
        state.tiers.set(tierLevel::NOST_WARRIOR, nostromoTier{ 30, 305.0 });
        state.tiers.set(tierLevel::NOST_QUEEN, nostromoTier{ 100, 1375.0 });

        //
        // Initialize key state variables.
        //
        state.transactionFee = 1000;
        state.stakedQubicsInContract = 0;
        state.projectNextId = 0;
        state.projectFee = 10000;

        //
        // Wallet of admin set at initialization
        //
        state.admin = qpi.invocator();
    _

};
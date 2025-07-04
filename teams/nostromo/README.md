# Nostromo Launchpad Smart Contract

## Team: Nostromo
**Members:**
- Bill (GitHub: @wfschrec)

## Overview
Nostromo Launchpad is a comprehensive platform for project funding and investment management on the Qubic blockchain. It implements a tier-based system that allows users to stake Qubic tokens to participate in project funding opportunities, with different tiers providing varying levels of investment privileges.

## Core Features

### 1. Tier System
- Five different investment tiers (EGG, DOG, ALIEN, WARRIOR, QUEEN)
- Each tier requires a different stake amount (1, 5, 10, 30, or 100 Qubic)
- Higher tiers provide greater investment weight in allocation calculations

### 2. Project Lifecycle Management
- Projects move through multiple states (DRAFT, PREPARE_VOTE, VOTE_STATE, etc.)
- Voting mechanism allows tier holders to vote on projects
- Multi-phase investment stages with configurable epoch durations

### 3. Investment Controls
- Min/max cap calculations based on project funding goals
- Investment tracking with per-user allocation
- Admin governance capabilities for project state transitions

## Smart Contract Functions

### User Tier Management
- `addUserTier(tierLevel tier)`: Allows a user to stake Qubic and join a specific tier
- `removeUserTier()`: Removes a user's tier and returns their staked funds

### Project Management
- `createProject(projectFinance financeInput)`: Creates a new funding project
- `getProject(uint64 projectId)`: Retrieves information about a project
- `changeProjectState(uint64 projectId, projectState newState)`: Admin function to transition projects between states

### Project Interaction
- `regForProject(uint64 projectIdentity)`: Register to participate in a project
- `unregForProject(uint64 projectIdentity)`: Unregister from a project
- `voteProject(uint64 projectId, voteValue vote)`: Vote on a project (YES or NO)
- `checkProjectVote(uint64 projectId)`: Check the current vote counts for a project

### Admin Controls
- `setPhaseOneEpochs(uint8 epochs)`: Configure duration of investment phase 1
- `setPhaseTwoEpochs(uint8 epochs)`: Configure duration of investment phase 2  
- `setPhaseThreeEpochs(uint8 epochs)`: Configure duration of investment phase 3

## Data Structures

- `nostromoTier`: Defines stake amount and pool weight for a tier
- `projectMeta`: Stores project metadata including votes and state
- `projectFinance`: Contains financial parameters for the project
- `projectCapPairs`: Tracks minimum and maximum investment caps

## State Tracking

The contract maintains several state variables and mappings:
- Tier definitions and user-to-tier mappings
- Total amount of Qubic staked in the contract
- Project metadata and financial information
- Transaction and project creation fees
- Project investment progress per phase
- User registration and voting activity

## Usage Example

1. Users join investment tiers by staking the required amount:
   ```
   // Stake 5 Qubic to join DOG tier
   addUserTier(NOST_DOG);
   ```

2. Project owners create new funding projects:
   ```
   // Create project with 100,000 Qubic target, 10% threshold
   projectFinance finance;
   finance.totalAmount = 100000;
   finance.threshold = 0.1;
   finance.tokenPrice = 50;
   createProject(finance);
   ```

3. Admin transitions projects through proper states:
   ```
   // Move project to voting phase
   changeProjectState(projectId, NOST_PREPARE_VOTE);
   ```

4. Users can register for projects they're interested in:
   ```
   // Register for a project
   regForProject(projectId);
   ```

5. Users vote on projects during voting phase:
   ```
   // Vote YES on a project
   voteProject(projectId, YES_VOTE);
   ```

6. Users can exit the platform by managing their tiers:
   ```
   // Exit the platform and retrieve staked funds
   removeUserTier();
   ```

## Implementation Notes

The contract implements strict validation rules to ensure users can only perform authorized actions. It manages state transitions carefully to maintain the integrity of the funding process, with precise tracking of all funds across the entire lifecycle.

We've designed the contract to be scalable, handling up to 131,072 users and 1,024 projects simultaneously. The implementation uses bit flags for efficient tracking of user registrations and votes across projects. 
// This is a test contract for your Qubic smart contract

using namespace qpi;

class SimpleContract {
public:
    // Stores balance at epoch start
    struct State {
        uint64_t previous_balance;
        uint64_t current_balance;
    };

    State state;

    // Function to update balance at BEGIN_EPOCH
    PUBLIC void begin_epoch() {
        ::Entity entity;
        bit status = qpi.getEntity(SELF, entity);

        require(status, "Failed to fetch entity data");

        state.previous_balance = state.current_balance;
        state.current_balance = entity.incomingAmount - entity.outgoingAmount;

        print("Previous Balance: ", state.previous_balance);
        print("Current Balance: ", state.current_balance);
    }

    // Function to check SC balance dynamically
    PUBLIC uint64_t get_balance() {
        ::Entity entity;
        bit status = qpi.getEntity(SELF, entity);
        require(status, "Failed to fetch entity balance");
        return entity.incomingAmount - entity.outgoingAmount;
    }

    // Function to deposit funds (test case)
    PUBLIC void deposit(uint64_t amount) {
        require(amount > 0, "Deposit amount must be positive");
        print("Received deposit: ", amount);
    }
}; 
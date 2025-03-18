# Echo and Burn Contract

## Team: ExampleTeam
**Members:**
- Jane Doe (GitHub: @janedoe)
- John Smith (GitHub: @johnsmith)
- Alice Johnson (GitHub: @alicej)

## Description
This contract demonstrates basic functionality in the Qubic smart contract ecosystem. It provides functions to echo (return) or burn funds sent to the contract, along with statistics tracking.

## Features
- Echo back funds sent to the contract
- Burn funds sent to the contract (remove them from circulation)
- Track statistics on contract usage

## Smart Contract Functions

### `Echo()`
Returns any Qubic sent with the transaction back to the sender.
- **Transaction Type:** 1
- **Requirements:**
  - None (works with zero value transactions)
- **Effects:**
  - Increments the echo call counter
  - Returns invocation amount to caller

### `Burn()`
Burns (permanently removes from circulation) any Qubic sent with the transaction.
- **Transaction Type:** 2
- **Requirements:**
  - None (works with zero value transactions)
- **Effects:**
  - Increments the burn call counter
  - Permanently removes sent Qubic from circulation

### `GetStats()`
Returns statistics about contract usage.
- **Function Type:** 1
- **Returns:**
  - Number of echo calls made to the contract
  - Number of burn calls made to the contract

## How to Test

To test this contract after deployment, use the following CLI commands:

1. Check your current balance:
   ```bash
   ./qubic-cli -nodeip YOUR_NODE_IP -seed YOUR_SEED -getbalance YOUR_IDENTITY
   ```

2. Call the Echo function with 1000 Qubic (will return the 1000 Qubic to you):
   ```bash
   ./qubic-cli -nodeip YOUR_NODE_IP -seed YOUR_SEED -sendcustomtransaction CONTRACT_IDENTITY 1 1000 0 0x
   ```

3. Call the Burn function with 1000 Qubic (will permanently burn the 1000 Qubic):
   ```bash
   ./qubic-cli -nodeip YOUR_NODE_IP -seed YOUR_SEED -sendcustomtransaction CONTRACT_IDENTITY 2 1000 0 0x
   ```

4. Get usage statistics (transaction type 1 function call):
   ```bash
   ./qubic-cli -nodeip YOUR_NODE_IP -seed YOUR_SEED -sendcustomtransaction CONTRACT_IDENTITY 1 0 0 0x -functionType
   ```

## Implementation Notes

This contract serves as a basic template and testing ground for Qubic smart contract functionality. It demonstrates:

1. State management with counters
2. Handling of funds sent to the contract
3. Different transaction types (echo and burn)
4. Query functions to access contract state

The contract is intentionally simple and serves as a starting point for more complex applications. It shows the fundamental patterns needed for Qubic smart contract development, including proper funds handling and state management. 
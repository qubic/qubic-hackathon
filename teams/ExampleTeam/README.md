# Simple Test Contract

## Team: ExampleTeam
**Members:**
- Jane Doe (GitHub: @janedoe)
- John Smith (GitHub: @johnsmith)
- Alice Johnson (GitHub: @alicej)

## Description
This is a simple test contract that demonstrates basic functionality in the Qubic smart contract ecosystem. The contract tracks balances across epochs and provides a deposit function for testing transaction interactions.

## Features
- Track contract balance across epochs
- Get current contract balance
- Process deposits (for testing purposes)

## Smart Contract Functions

### `begin_epoch()`
Called at the beginning of each epoch to update the contract's balance record.
- **Requirements:**
  - Must successfully fetch entity data
- **Effects:**
  - Updates previous_balance with the last known balance
  - Sets current_balance to the latest contract balance
  - Prints both values for verification

### `get_balance()`
Returns the current balance of the contract.
- **Returns:**
  - Current contract balance (uint64_t)
- **Requirements:**
  - Must successfully fetch entity data

### `deposit(uint64_t amount)`
Test function that accepts deposits to the contract.
- **Parameters:**
  - `amount`: The amount of Qubic to deposit
- **Requirements:**
  - Amount must be greater than zero
- **Effects:**
  - Prints confirmation message with deposit amount

## How to Test

To test this contract after deployment, use the following CLI commands:

1. Check your current balance:
   ```bash
   ./qubic-cli -nodeip YOUR_NODE_IP -seed YOUR_SEED -getbalance YOUR_IDENTITY
   ```

2. Send a deposit of 1000 Qubic to the contract:
   ```bash
   ./qubic-cli -nodeip YOUR_NODE_IP -seed YOUR_SEED -sendtoaddress CONTRACT_IDENTITY 1000
   ```

3. Check contract balance (transaction type 1):
   ```bash
   ./qubic-cli -nodeip YOUR_NODE_IP -seed YOUR_SEED -sendcustomtransaction CONTRACT_IDENTITY 1 0 0 0x
   ```

4. Make a deposit via contract function (transaction type 2):
   ```bash
   ./qubic-cli -nodeip YOUR_NODE_IP -seed YOUR_SEED -sendcustomtransaction CONTRACT_IDENTITY 2 1000 0 0x
   ```

## Implementation Notes

This contract serves as a basic template and testing ground for Qubic smart contract functionality. It demonstrates:

1. State management across epochs
2. Dynamic balance checking
3. Basic transaction handling

The contract is intentionally simple and serves as a starting point for more complex applications. It shows the fundamental patterns needed for Qubic smart contract development, including proper entity balance fetching and validation. 
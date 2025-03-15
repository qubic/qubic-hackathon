# Qubic Hackathon Madrid - March 22-23, 2024

## Table of Contents
- [Overview](#overview)
- [What is Qubic?](#what-is-qubic)
- [Hackathon Process](#hackathon-process)
  - [Why Submit via Pull Request?](#why-submit-via-pull-request)
  - [Team Participation](#team-participation)
  - [Smart Contract Development & Deployment Process](#smart-contract-development--deployment-process)
  - [Pull Request Instructions](#pull-request-instructions)
- [Development Resources](#development-resources)
  - [Smart Contract Development](#smart-contract-development)
  - [Best Practices](#best-practices-for-qubic-smart-contracts)
  - [Online Development Tools](#online-development-tools)
  - [Qubic Wallets](#qubic-wallets)
  - [Qubic CLI Commands Reference](#qubic-cli-commands-reference)
- [Technical Infrastructure](#technical-infrastructure)
  - [RPC Endpoints](#rpc-endpoints)
  - [Faucet](#faucet)
  - [TypeScript Library](#typescript-library)
  - [Smart Contract Examples](#smart-contract-examples)
- [Project Structure](#project-structure)
- [FAQs](#faqs)
- [Contact](#contact)

## Overview
Welcome to the Qubic Hackathon Madrid repository! This hackathon will take place on March 22-23, 2024, bringing together developers to build innovative smart contracts on the Qubic blockchain. This repository serves as the central hub for participants to submit their smart contracts, access resources, and interact with the infrastructure team.

## What is Qubic?
Qubic is a blockchain platform that enables the creation and execution of smart contracts. Qubic smart contracts are self-executing programs that run within the Qubic ecosystem, automating logic, managing balances, and interacting with the blockchain to process transactions securely and efficiently.

### Key Features of Qubic Smart Contracts
- Each smart contract has its own balance – funds sent to the SC are tracked internally
- Transactions interact with the SC – functions in the SC execute based on received inputs
- Epoch-based execution – Qubic SCs operate in epoch cycles (BEGIN_EPOCH and END_EPOCH events)
- State persistence – Qubic SCs use state storage to persist data across transactions

## Hackathon Process

### Why Submit via Pull Request?
Normally, deploying a Qubic smart contract requires setting up and running your own testnet node, which is complex and time-consuming. For this hackathon, we've simplified the process:

1. You focus on writing your smart contract
2. You submit it via a pull request to this repository
3. Our infrastructure team will:
   - Review your code
   - Deploy it to its own dedicated node (one node per smart contract)
   - Provide you with the access details to your team's specific node
   
This way, you can concentrate on development without worrying about node operation and infrastructure.

### Team Participation
- Each team will develop their own smart contract
- The infrastructure team will support deployment and testing

### Smart Contract Development & Deployment Process
1. Teams develop their smart contracts using C++
2. Teams create a pull request to this repository with their smart contract code
   - Name the PR with your team name and contract name
3. The infrastructure team will review the PR
   - If issues are found, comments will be added to the PR for your team to address
   - Once fixed, resubmit your changes
4. Upon successful compilation, the infrastructure team will:
   - Deploy your contract to its own dedicated node (each smart contract gets its own unique node)
   - Provide you with the IP address of your contract's dedicated node
   - Provide seeds with test Qubic funds for experimenting
5. Teams can then interact with their deployed contracts using the Qubic CLI or online CLI IDE
6. If updates are needed, open a new PR with your changes

### Pull Request Instructions
When your team is ready to submit or update a smart contract, follow these steps:

1. **Fork this repository**
   - Click the "Fork" button at the top right of this repository
   - This creates a copy of the repository in your GitHub account

2. **Clone your fork**
   ```bash
   git clone https://github.com/YOUR-USERNAME/hackathon-madrid.git
   cd hackathon-madrid
   ```

3. **Create your team directory**
   ```bash
   mkdir -p teams/YOUR-TEAM-NAME
   ```

4. **Add your smart contract files**
   - Create your contract file: `teams/YOUR-TEAM-NAME/contract.h`
   - Create a README explaining your contract: `teams/YOUR-TEAM-NAME/README.md`
   - Include any other necessary files

5. **Commit and push your changes**
   ```bash
   git add teams/YOUR-TEAM-NAME
   git commit -m "Add/Update smart contract for Team YOUR-TEAM-NAME"
   git push origin main
   ```

6. **Create a Pull Request**
   - Go to your forked repository on GitHub
   - Click "Pull Request"
   - Set the title as: "Team [YOUR-TEAM-NAME]: [Contract Name]"
   - Provide a description of your contract and what it does
   - Submit the pull request

7. **Review Process**
   - The infrastructure team will review your submission
   - If there are issues, they will add comments to your PR
   - Make any requested changes and push to your fork again
   - Once approved, your contract will be deployed to a test node

8. **Contract Deployment**
   - After approval, you'll receive a comment with:
     - The IP address of your dedicated node where your contract is deployed
     - Seeds for accessing:
       - A specific seed for your team's smart contract node
     - Any specific instructions for your contract

   > **Important:** Since Qubic does not currently have a public testnet for smart contracts, our infrastructure team will compile your contract on its own dedicated node. Each team's smart contract will run on a separate node with its own IP address. You'll use the CLI commands with your node's IP to interact with your specific contract.

9. **Contract Updates**
   - If you need to update your contract, create a new PR following the same process
   - Clearly indicate in the PR description that this is an update to an existing contract

## Development Resources

### Smart Contract Development
Qubic smart contracts are written in C++ with specific structures and patterns. Below is a simple example:

```cpp
using namespace qpi;

class SimpleQubicContract {
public:
    struct State {
        uint64_t totalDeposits;
        uint64_t lastEpochBalance;
    };

    State state;

    // Deposit function
    PUBLIC void deposit(uint64_t amount) {
        require(amount > 0, "Deposit must be positive");
        state.totalDeposits += amount;
        print("Deposited: ", amount);
    }

    // Fetch SC balance dynamically
    PUBLIC uint64_t get_balance() {
        ::Entity entity;
        bit status = qpi.getEntity(SELF, entity);
        require(status, "Failed to fetch balance");
        return entity.incomingAmount - entity.outgoingAmount;
    }

    // Epoch event handler
    PUBLIC void begin_epoch() {
        ::Entity entity;
        bit status = qpi.getEntity(SELF, entity);
        require(status, "Failed to fetch entity");
        state.lastEpochBalance = entity.incomingAmount - entity.outgoingAmount;
        print("Updated balance for new epoch.");
    }
};
```

### Best Practices for Qubic Smart Contracts

1. **Smart Contract Initialization**
   - Always initialize all state variables explicitly

2. **Managing Smart Contract Balances**
   - Fetch SC balance dynamically using `qpi.getEntity()`
   - Never assume a fixed amount—always track incoming and outgoing funds

3. **Handling Transactions Correctly**
   - Ensure transactions are properly validated before modifying state

4. **Using State Variables Correctly**
   - Store all necessary data in state—Qubic SCs do not retain local variables between calls

5. **Security Best Practices**
   - Use `require()` conditions before modifying state
   - Validate input values
   - Use proper balance fetching methods

### Online Development Tools

#### Smart Contract IDE
For development and testing, you can use the online [Smart Contract IDE](https://qubicdev.com/smart-contract-tester.html) which provides:
- Syntax highlighting and code completion
- AI-powered suggestions for optimization and security improvements

### Smart Contract Examples
For smart contract examples, you can explore the official Qubic smart contracts:

- [Official Qubic Smart Contracts](https://github.com/qubic/core/tree/main/src/contracts)
- [Contract Core Functions](https://github.com/qubic/core/tree/main/src/contract_core)
- [Platform Functions](https://github.com/qubic/core/tree/main/src/platform)

These examples can be viewed and tested in the [Smart Contract IDE](https://qubicdev.com/smart-contract-tester.html).

Since our core smart contract library is still in development, you can utilize functions from both the contract core and platform libraries. If you encounter a missing function, feel free to add it to the appropriate core or platform files. For any questions, post them on the dev channel for guidance.

Some notable examples include:
- Asset management contracts
- Staking contracts
- Multi-signature wallets
- Trading mechanisms

#### CLI IDE
For interacting with deployed contracts, you can use the online [CLI IDE](https://qubicdev.com/index.html) which offers:
- Execute Qubic CLI commands directly from the browser
- Save and manage code files for your projects
- AI assistance for generating optimal commands and troubleshooting

If you prefer to run the CLI code locally, you can do so by following the instructions on the [Qubic CLI GitHub](https://github.com/qubic/qubic-cli). For Mac OS Silicon chips, use the branch [2024-12-18-port-aarch64](https://github.com/qubic/qubic-cli/tree/2024-12-18-port-aarch64).

These CLI commands are used to call and test the smart contract (SC), and the AI can help create the commands using the online IDE.


### Qubic Wallets - Pending

For interacting with the Qubic blockchain, you can use:

#### MetaMask Snap
You can connect to Qubic through MetaMask using the Qubic Snap:
- Follow the installation instructions at [Qubic MetaMask Snap](https://metamask.io/snaps/)
- This allows you to manage your Qubic assets directly through MetaMask

#### Wallet Connect
Qubic supports WalletConnect protocol for connecting your wallet to dApps:
- Enable WalletConnect in your Qubic-compatible wallet
- Use the connection string provided by dApps to establish a secure connection

These wallets can be used to interact with your smart contracts once they're deployed.

### Qubic CLI Commands Reference

Here are some essential CLI commands you'll need:

- `./qubic-cli -help`: Prints the help message with all available commands
- `./qubic-cli -nodeip <IPv4_ADDRESS>`: Specifies the IP address of the target node
- `./qubic-cli -nodeport <PORT>`: Specifies the port of the target node
- `./qubic-cli -getbalance <IDENTITY>`: Retrieves the balance of a specified identity
- `./qubic-cli -sendtoaddress <TARGET_IDENTITY> <AMOUNT>`: Sends Qubic to a target identity
- `./qubic-cli -gettxinfo <TX_ID>`: Gets transaction information
- `./qubic-cli -getcurrenttick`: Fetches the current tick information of the node

For a complete list of commands, visit the [Qubic CLI Readme](https://github.com/qubic/qubic-cli).

## Technical Infrastructure

### RPC Endpoints
Qubic provides RPC (Remote Procedure Call) endpoints for interacting with the blockchain:

- **Mainnet RPC**: https://rpc.qubic.org
- **Testnet RPC**: https://testnet-rpc.qubic.org

These endpoints can be used with the Qubic TypeScript library or direct API calls to communicate with the Qubic network.

#### Available RPC Services and documentation:
- [Qubic Archive Tree](https://qubic.github.io/integration/Partners/qubic-rpc-doc.html?urls.primaryName=Qubic%20RPC%20Archive%20Tree) - For historical data access
- [Qubic Live Tree](https://qubic.github.io/integration/Partners/qubic-rpc-doc.html?urls.primaryName=Qubic%20RPC%20Live%20Tree) - For real-time data access
- [Qubic Transfers API](https://qubic.github.io/integration/Partners/qubic-rpc-doc.html?urls.primaryName=Qubic%20Transfers%20API) - For transfer-specific operations

#### Available RPC Services
The Qubic RPC API offers a variety of endpoints for different use cases, as detailed in the [official documentation](https://docs.qubic.org/api/rpc/) and [Swagger archive](https://qubic.github.io/integration/Partners/qubic-rpc-doc.html?urls.primaryName=Qubic%20RPC%20Archive%20Tree). Key endpoints include:

1. **General Network Information**
   - `GET /v1/status`: Retrieves the latest RPC server statistics, including:
     - `timestamp`: Current timestamp (e.g., "1724325433")
     - `circulatingSupply`: Total QUs in circulation (e.g., "109929085175710")
     - `activeAddresses`: Number of active addresses (e.g., 477228)
     - `price`: Current QU price in USD (e.g., 0.000001743)
     - `marketCap`: Market capitalization in USD (e.g., "191606393")
   - `GET /v1/tick`: Returns the current tick (block height) of the network.

2. **Transaction Handling**
   - `POST /v1/broadcast-transaction`: Broadcasts a transaction to the network.
   - `GET /v1/tick-transactions/{tick}`: Retrieves a list of approved transactions for a given tick.
   - `GET /v1/transaction/{txId}`: Fetches details of a specific transaction.
   - `GET /v1/transaction-status/{txId}`: Checks the status of a specific transaction.

3. **Account and Balance Queries**
   - `GET /v1/balance/{addressId}`: Retrieves the balance for a specified address ID.
   - `GET /v1/transfers/{identity}/{fromTick}/{toTick}`: Lists transfer transactions for an identity within a tick range.

4. **Advanced Blockchain Data**
   - `GET /v1/tick-info/{tick}`: Provides tick information, including timestamp, epoch, and included transaction IDs.
   - `GET /v1/chain-hash/{tick}`: Returns the chain hash for a specific tick.
   - `GET /v1/quorum-tick/{tick}`: Retrieves quorum tick data for a specific tick.
   - `GET /v1/store-hash/{tick}`: Gets the store hash for a specific tick.

#### Using RPC Endpoints
These endpoints can be accessed via HTTP requests (e.g., using `curl`, a TypeScript library, or any HTTP client). Example using `curl`:

```bash
curl https://rpc.qubic.org/v1/status
```

You can build applications that interact with Qubic using these RPC endpoints, without the need to run your own node.

For detailed specifications and response formats, refer to the [Qubic RPC Swagger Documentation](https://docs.qubic.org/api/rpc/#swagger-api-documentation).

#### Smart Contract Interaction Limitations
**Important:** The current RPC endpoints **do not support direct calls to new deployed smart contract functions**. This means you cannot invoke or interact with smart contracts (new or existing) using the RPC API alone. Instead:

- **New Smart Contracts**: For this hackathon, deploying and interacting with newly developed smart contracts requires the Qubic CLI. After your contract is deployed to its dedicated node (via pull request), you'll use CLI commands with the provided node IP to call its functions (e.g., `deposit`, `get_balance`). See the [Qubic CLI Commands Reference](#qubic-cli-commands-reference) for details.
- **Existing Smart Contracts**: For challenges that don't involve creating a new smart contract, you can leverage the RPC endpoints to interact with the broader Qubic network (e.g., querying balances, transactions, or ticks) and then use CLI commands to call functions of pre-deployed smart contracts on your assigned node.

#### Hackathon Development Guidance
- **For Challenges Requiring New Smart Contracts**: Focus on writing your C++ smart contract code and submitting it via pull request. Post-deployment, use the CLI with your node's IP to test and interact with it.
- **For Challenges Using Existing Contracts**: You can use the RPC endpoints (e.g., `/v1/balance`, `/v1/transfers`) to fetch network data and build applications around existing smart contracts. Then, use the CLI to call specific functions of those contracts as needed, referencing their node IP.

For further integration details, explore the [Qubic Integration GitHub](https://qubic.github.io/integration/Partners/qubic-rpc-doc.html).

### Faucet
Need test funds for development? You can access the Qubic faucet:

1. Join the [Qubic Discord](https://discord.gg/qubic)
2. Navigate to the `#bot-commands` channel
3. Use the faucet command to receive:
   - 1000 Qubics on mainnet
   - Test Qubics for the testnet RPC

This will provide you with the necessary funds to test your smart contracts during development.

### TypeScript Library
Qubic provides an official TypeScript library to interact with the Qubic network programmatically:

- [Qubic TypeScript Library](https://github.com/qubic/ts-library/tree/main)
- [Examples](https://github.com/qubic/ts-library/tree/main/test)

#### Installation
```bash
yarn add @qubic-lib/qubic-ts-library
```
or
```bash
npm install @qubic-lib/qubic-ts-library
```

#### Basic Usage
```typescript
// Import helper
import { QubicHelper } from 'qubic-ts-library/dist/qubicHelper'

// Create an ID Package with private/public key and human readable address
const id = await helper.createIdPackage("your-seed-phrase");

// Connect to a node
import { QubicConnector } from 'qubic-ts-library/dist/qubicConnector'
const connector = new QubicConnector("https://rpc.qubic.org");

// Get balance
const balance = await connector.getBalance("IDENTITY_HERE");
```

Check the [examples directory](https://github.com/qubic/ts-library/tree/main/test) for more usage scenarios, including:
- Creating transactions
- Fetching balances
- Managing multiple users
- Handling deposits/withdrawals


## Project Structure
```
/
├── README.md                  # This file - general information
└── teams/                     # Directory containing team submissions
    └── {team-name}/           # Your team directory (create during PR)
        ├── contract.h       # Smart contract code
        └── README.md          # Description of your contract
```

## FAQs

**Q: Do I need to set up my own node?**  
A: No. Our infrastructure team will deploy your smart contract on a pre-configured test node. You only need to focus on developing your smart contract.

**Q: How do I test my smart contract?**  
A: After your contract is deployed, you'll be provided with a node IP address and a wallet with test funds. You can then use the CLI IDE to interact with your contract.

**Q: Can I update my smart contract after it's deployed?**  
A: Yes. Submit a new pull request with your updated contract code. The infrastructure team will review, deploy the updated version, and provide you with the new deployment details.

**Q: How do I know if my smart contract has issues?**  
A: The infrastructure team will review your pull request and add comments if there are any issues with compilation or deployment.

**Q: Can I build a frontend app that interacts with my smart contract?**  
A: Yes! You can use the RPC endpoints and TypeScript library to build a frontend application that interacts with your deployed smart contract.

**Q: Do I have to use C++ for my smart contract?**  
A: Yes. At this time, Qubic smart contracts must be written in C++.

**Q: How is the testnet for smart contracts set up?**  
A: Unlike the main Qubic testnet (accessible via RPC), we don't have a public testnet specifically for smart contracts. Instead, our infrastructure team will compile and deploy each smart contract to its own dedicated node. Every smart contract in the hackathon will have a unique node with its own IP address. You'll receive the IP address of your contract's node to interact with it using CLI commands.

**Q: Will I get seeds/faucet for testing?**  
A: Yes, you'll receive three types of seeds/faucet:
1. Faucet with mainnet Qubic funds
2. Faucet with testnet Qubic funds (for the RPC testnet)
3. A unique seed for interacting with your team's smart contract node

**Q: How do I verify my smart contract is working?**  
A: After deployment, use the provided seed and node IP with the Qubic CLI (or online CLI IDE) to send commands to your contract. For example:
```bash
./qubic-cli -nodeip YOUR_NODE_IP -seed YOUR_SEED -somecommand
```

## Contact
If you have any questions or need assistance during the hackathon, please contact the infrastructure team through [contact information].

---

*This repository and documentation were created for the Qubic Hackathon Madrid, March 22-23, 2024.*

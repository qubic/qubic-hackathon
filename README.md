# Qubic Hackathon Madrid - March 22-23, 2024

## Table of Contents
- [Overview](#overview)
- [Quick Start](#quick-start)
- [What is Qubic?](#what-is-qubic)
- [Hackathon Process](#hackathon-process)
  - [Development Process Overview](#development-process-overview)
  - [Team Participation](#team-participation)
  - [Smart Contract Development & Deployment Process](#smart-contract-development--deployment-process)
  - [SSH Connection Instructions](#ssh-connection-instructions)
- [Development Resources](#development-resources)
  - [Smart Contract Development](#smart-contract-development)
  - [Best Practices](#best-practices-for-qubic-smart-contracts)
  - [Online Development Tools](#online-development-tools)
  - [Qubic CLI Commands Reference](#qubic-cli-commands-reference)
- [Technical Infrastructure](#technical-infrastructure)
  - [RPC Endpoints](#rpc-endpoints)
  - [Faucet](#faucet)
  - [Available Testnet Seeds](#available-testnet-seeds)
  - [TypeScript Library](#typescript-library)
  - [Smart Contract Examples](#smart-contract-examples)
- [Frontend Application](#frontend-application)
  - [Frontend Overview](#frontend-overview)
  - [Frontend Features](#frontend-features)
  - [Connecting to a Node](#connecting-to-a-node)
  - [Smart Contract Interaction Examples](#smart-contract-interaction-examples)
- [Project Structure](#project-structure)
- [FAQs](#faqs)
- [Contact](#contact)

## Overview
Welcome to the Qubic Hackathon Madrid repository! This hackathon will take place on March 22-23, 2024, bringing together developers to build innovative smart contracts on the Qubic blockchain. This repository serves as the central hub for participants to submit their smart contracts, access resources, and interact with the infrastructure team.

## Quick Start

1. **Join the Qubic Discord Community**: [discord.gg/qubic](https://discord.gg/qubic)

2. **Get Access to a Test Node**:
   - Reach out in the `#dev` channel to get SSH access to a dedicated testnet node
   - Deploy the HM25 template smart contract using the provided scripts

3. **Customize Your Smart Contract**:
   - Fork the [Qubic Core repository](https://github.com/qubic/core) and switch to the `madrid-2025` branch
   - Modify the `src/contracts/HM25.h` template file with your own business logic
   - Keep the existing function structure but add your own state variables and procedures
   - Use the provided deployment script to deploy your modified contract

4. **Explore the Example dApp**:
   - Check out [https://github.com/icyblob/hm25-frontend](https://github.com/icyblob/hm25-frontend) - a complete example dApp
   - Features full wallet integrations (MetaMask Snap, WalletConnect, seed phrase, Vault file)
   - Demonstrates real-world smart contract interactions with the HM25 template
   - Use this as a reference implementation for your own Qubic dApps

5. **Connect to Your Node**:
   - The app connects to a testnet node by default
   - To connect to your own node, use the ConnectLink feature in the top right
   - Use format: `http://<your-node-ip>` (no port needed)

6. **Interact with Your Smart Contract**:
   - Use the frontend to call your SC functions
   - View contract statistics
   - Use provided test seeds to sign transactions

## What is Qubic?
Qubic is a blockchain platform that enables the creation and execution of smart contracts. Qubic smart contracts are self-executing programs that run within the Qubic ecosystem, automating logic, managing balances, and interacting with the blockchain to process transactions securely and efficiently.

### Key Features of Qubic Smart Contracts
- Each smart contract has its own balance – funds sent to the SC are tracked internally
- Transactions interact with the SC – functions in the SC execute based on received inputs
- Epoch-based execution – Qubic SCs operate in epoch cycles (BEGIN_EPOCH and END_EPOCH events)
- State persistence – Qubic SCs use state storage to persist data across transactions

## Hackathon Process

### Development Process Overview
The Qubic ecosystem is complex to set up from scratch. To simplify development for this hackathon, we're providing:

1. A dedicated test node accessible via SSH
2. A deployment script that sets up the entire Qubic ecosystem in one machine with a single command
3. A pre-approved smart contract template (HM25.h) that you can modify

This approach means you can focus on writing your smart contract code without worrying about the complex infrastructure required to run the Qubic network.

### Team Participation
- Each team will develop their own smart contract
- The team will support deployment and testing

### Smart Contract Development & Deployment Process
1. Teams develop their smart contracts using C++
2. Fork the Qubic core repository from https://github.com/qubic/core
3. Switch to the `madrid-2025` branch which contains the optimized test node version
4. Develop your smart contract by editing the `HM25.h` template file
   - This is the template smart contract we've prepared for the hackathon
   - You can modify this file with your SC code
   - Note: On Qubic mainnet, each SC needs to be IPOed and approved by computors, but for this hackathon, we've simplified the process with a pre-approved template contract
5. Advanced developers can also add functions in files under `contract_core` and `contracts` folders, but this is not recommended for beginners
6. To test your smart contract on a dedicated test node:
   - Contact the team on the `#dev` channel on Discord and we will provide access
   - Connect to the test node via SSH (Visual Studio Code's SSH extension is recommended so you can see and modify files directly)
   - Run the deployment script: 
     ```bash
     ./deploy.sh https://github.com/{your-username}/core/tree/madrid-2025
     ```
     in the `/root/qubic/qubic-docker` folder
   - The script will start the node, send indices to get the network ticking, and deploy RPC
   - Note: Your repository must be public, not private, for this to work
7. If deployment is successful, you'll see:
   ```
   Deployment completed successfully.
   RPC is available at: http://185.84.224.158/tick-info
   To connect to the testnet via qubic-cli, use:
   _______________________
   |                     |
   | IP: 185.84.224.158  |
   | Port: 31841         |
   |_____________________|
   Example commands:
   ./qubic-cli -nodeip 185.84.224.158 -nodeport 31841 -getcurrenttick
   Response:
   Tick: 21190235
   Epoch: 152
   Number Of Aligned Votes: 0
   Number Of Misaligned Votes: 0
   Initial tick: 21190000
   ./qubic-cli -nodeip 185.84.224.158 -nodeport 31841 -getbalance WEVWZOHASCHODGRVRFKZCGUDGHEDWCAZIZXWBUHZEAMNVHKZPOIZKUEHNQSJ
   Response:
   Identity: WEVWZOHASCHODGRVRFKZCGUDGHEDWCAZIZXWBUHZEAMNVHKZPOIZKUEHNQSJ
   Balance: 1000000000
   Incoming Amount: 1000000000
   Outgoing Amount: 0
   Number Of Incoming Transfers: 1
   Number Of Outgoing Transfers: 0
   Latest Incoming Transfer Tick: 15502487
   Latest Outgoing Transfer Tick: 0
   Tick: 21190235
   Spectum Digest: c747adefec62c38dd4a64d554f72723daf71aded23ea5782625ff3cf16e49ec2
   =======================================================================================================================
   ```
8. If deployment fails, review the logs to identify and fix issues with your smart contract
9. After successful deployment, test your smart contract using the Qubic CLI
   - You can use the online CLI at qubicdev.com or run the CLI locally
   - For Mac OS Silicon chips, use the branch specified in the CLI section of this README
10. Build a frontend application that interacts with your smart contract through the testnet node's RPC
11. When you've completed your smart contract and application, notify us on the `#dev` channel on Discord for feedback and review

> **Important:** Each team's testnet node will have its own IP address and RPC endpoint. This allows you to call directly to your specific node. We've set up the testnet nodes to make the process simpler so teams can focus on building rather than dealing with the complexities of running a Qubic node.

> **Critical:** Every time you modify your smart contract, you MUST first run the cleanup command `./cleanup.sh` and then redeploy using `./deploy.sh`. If you don't do this, the node will continue running with your old code changes.

### SSH Connection Instructions
To access your dedicated testnet node for smart contract development:

1. **Request access credentials**
   - Reach out to the team on the `#dev` channel on Discord
   - We will provide you with:
     - IP address: `66.248.204.226`
     - Username: `root`
     - Password: Will be provided securely via Discord

2. **Connect via SSH**
   We recommend using Visual Studio Code with the SSH extension for the best experience:
   
   a. Install the "Remote - SSH" extension in VS Code
   
   b. Click on the green icon in the bottom-left corner
   
   c. Select "Connect to Host..." and add a new SSH host:
   ```
   ssh root@66.248.204.226
   ```
   
   d. Enter the password when prompted
   
   e. Browse to the `/root/qubic/qubic-docker` directory to find the deployment scripts

3. **Deploy your forked repository**
   Run the deployment script with your GitHub repository URL:
   ```bash
   ./deploy.sh https://github.com/{your-username}/core/tree/madrid-2025
   ```
   
   **Note:** Ensure your repository is public, as private repositories will not work with this script.

4. **Monitor deployment**
   - Successful deployment will display RPC endpoint information
   - Failed deployments will show error logs to help you troubleshoot

5. **Monitor transaction logs**
   After successful deployment, you can monitor real-time transaction logs using the qlogging service:
   ```bash
   ./qubic/scripts/qlogging 127.0.0.1 31841 1 2 3 4 21180000
   ```
   
   Run this command in a separate terminal tab to see real-time logs of transactions, transfers, and other events happening on your node. The output will look like:
   ```
   EventTx #1FromId Told1 2[1] 21183461.153 QU transfer: from WTUBWAEQJHTFIEDXCJHVRXAXYBFCHAPQUPOQMGTJVGXYEBVRYTOVFHLFBCMB to MAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAWWD 10000QU.
   Tick 21183462 doesn't generate any log
   Tick 21183463 doesn't generate any log
   ...
   [4] 21183473.153 Burn: MAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAWLWD burned 100000 QU
   ```
   
   This is invaluable for debugging and understanding how your smart contract interacts with the network.

6. **Shutdown procedure**
   When you're done working, clean up your deployment:
   ```bash
   ./cleanup.sh
   ```

## Development Resources

### Smart Contract Development
Qubic smart contracts are written in C++ with specific structures and patterns. Below is an example based on our template:

```h
using namespace QPI;

struct HM252
{
};

struct HM25 : public ContractBase
{
public:
    struct Echo_input{};
    struct Echo_output{};

    struct Burn_input{};
    struct Burn_output{};

    struct GetStats_input {};
    struct GetStats_output
    {
        uint64 numberOfEchoCalls;
        uint64 numberOfBurnCalls;
    };

private:
    uint64 numberOfEchoCalls;
    uint64 numberOfBurnCalls;

    /**
    Send back the invocation amount
    */
    PUBLIC_PROCEDURE(Echo)
        state.numberOfEchoCalls++;
        if (qpi.invocationReward() > 0)
        {
            qpi.transfer(qpi.invocator(), qpi.invocationReward());
        }
    _

    /**
    * Burn all invocation amount
    */
    PUBLIC_PROCEDURE(Burn)
        state.numberOfBurnCalls++;
        if (qpi.invocationReward() > 0)
        {
            qpi.burn(qpi.invocationReward());
        }
    _

    PUBLIC_FUNCTION(GetStats)
        output.numberOfBurnCalls = state.numberOfBurnCalls;
        output.numberOfEchoCalls = state.numberOfEchoCalls;
    _

    REGISTER_USER_FUNCTIONS_AND_PROCEDURES

        REGISTER_USER_PROCEDURE(Echo, 1);
        REGISTER_USER_PROCEDURE(Burn, 2);

        REGISTER_USER_FUNCTION(GetStats, 1);
    _

    INITIALIZE
        state.numberOfEchoCalls = 0;
        state.numberOfBurnCalls = 0;
    _
};
```

This template demonstrates:
- Input/output structures for functions
- Procedures that can modify state
- State tracking (counters for function calls)
- Function registration
- State initialization

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

### Smart Contract Examples
For smart contract examples, you can explore the official Qubic smart contracts:

- [Official Qubic Smart Contracts](https://github.com/qubic/core/tree/main/src/contracts)
- [Contract Core Functions](https://github.com/qubic/core/tree/main/src/contract_core)
- [Platform Functions](https://github.com/qubic/core/tree/main/src/platform)

Since our core smart contract library is still in development, you can utilize functions from both the contract core and platform libraries. If you encounter a missing function, feel free to add it to the appropriate core or platform files. For any questions, post them on the dev channel for guidance.

Some notable examples include:
- Asset management contracts
- Staking contracts
- Multi-signature wallets
- Trading mechanisms

#### CLI IDE
For interacting with deployed contracts, you can use the online [CLI IDE](https://qubicdev.com/) which offers:
- Execute Qubic CLI commands directly from the browser
- Save and manage code files for your projects
- AI assistance for generating optimal commands and troubleshooting

If you prefer to run the CLI code locally, you can do so by following the instructions on the [Qubic CLI GitHub](https://github.com/qubic/qubic-cli). For Mac OS Silicon chips, use the branch [2024-12-18-port-aarch64](https://github.com/qubic/qubic-cli/tree/2024-12-18-port-aarch64).

These CLI commands are used to call and test the smart contract (SC), and the AI can help create the commands using the online IDE.


### Qubic CLI Commands Reference

Here are some essential CLI commands you can use need:

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
- **Testnet Node IP**: (example: http://66.248.204.226/v1/tick-info)

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
**Important:** The RPC endpoints have varying levels of support for smart contract interactions:

- **Pre-deployed HM25 Template**: The example frontend application demonstrates how to interact with the pre-deployed HM25 template contract using RPC endpoints like `/v1/querySmartContract` for reading data and `/v1/broadcast-transaction` for executing procedures.

- **Your Custom Contracts**: When you modify and deploy your own version of the smart contract, initially you may need to use the Qubic CLI to test your custom functions. After verifying your contract is working correctly, you can integrate it with the frontend following the patterns shown in the example application.

- **RPC vs CLI Usage**: For most teams, the example frontend application provides all necessary components to interact with your smart contract through RPC. The CLI is most useful for debugging and initial testing.

For further integration details, explore the [Qubic Integration GitHub](https://qubic.github.io/integration/Partners/qubic-rpc-doc.html).

### Faucet
Need test funds for development? You can access the Qubic faucet:

1. Join the [Qubic Discord](https://discord.gg/qubic)
2. Navigate to the `#bot-commands` channel
3. Use the faucet command to receive:
   - 1000 Qubics on mainnet
   - Test Qubics for the testnet RPC

This will provide you with the necessary funds to test your smart contracts during development.

### Available Testnet Seeds
The testnet node comes with pre-funded seeds that you can use for testing. Each seed contains 1 billion Qubic tokens. Here's the list of available seeds:

```
fwqatwliqyszxivzgtyyfllymopjimkyoreolgyflsnfpcytkhagqii
xpsxzzfqvaohzzwlbofvqkqeemzhnrscpeeokoumekfodtgzmwghtqm
ukzbkszgzpipmxrrqcxcppumxoxzerrvbjgthinzodrlyblkedutmsy
wgfqazfmgucrluchpuivdkguaijrowcnuclfsjrthfezqapnjelkgll
kewgvatawujuzikurbhwkrisjiubfxgfqkrvcqvfvgfgajphbvhlaos
nkhvicelolicthrcupurhzyftctcextifzkoyvcwgxnjsjdsfrtbrbl
otyqpudtgogpornpqbjfzkohralgffaajabxzhneoormvnstheuoyay
ttcrkhjulvxroglycvlpgesnxpwgjgvafpezwdezworzwcfobevoacx
mvssxxbnmincnnjhtrlbdffulimsbmzluzrtbjqcbvaqkeesjzevllk
jjhikmkgwhyflqdszdxpcjrilnoxerfeyttbbjahapatglpqgctnkue
nztizdwotovhuzchctpfdgylzmsdfxlvdcpikhmptqjbwwgbxavhtwo
lxbjeczdoqyjtzhizbeapkbpvfdbgxxbdbhyfvzhbkysmgdxuzspmwu
zwoggmzfbdhuxrikdhqrmcxaqmpmdblgsdjzlesfnyogxquwzutracm
inkzmjoxytbhmvuuailtfarjgooearejunwlzsnvczcamsvjlrobsof
htvhtfjxzqandmcshkfifmrsrikrcpsxmnemcjthtmyvsqqcvwckwfk
hmsmhamftvncxcdvxytqgdihxfncarwzatpjuoecjqhceoepysozwlp
wrnohgpgfuudvhtwnuyleimplivlxcaswuwqezusyjddgkdigtueswb
fisfusaykkovsskpgvsaclcjjyfstrstgpebxvsqeikhneqaxvqcwsf
jftgpcowwnmommeplhbvgotjxrtkmiddcjmitbxoekwunmlpmdakjzq
svaluwylhjejvyjvgmqsqjcufulhusbkkujwrwfgdphdmesqjirsoep
lzinqhyvomjzqoyluifguhytcgpftdxndswbcqriecatcmfidbnmvka
mqamjotnshocvekufdqylgtdcembtddlfockjyaotfdvzqpvkylsjjk
asueorfnexvnthcuicsqqppekcdrwizxqlnkzdkazsymrotjtmdnofe
ahfulnoaeuoiurixbjygqxiaklmiwhysazqylyqhitjsgezhqwnpgql
omyxajeenkikjvihmysvkbftzqrtsjfstlmycfwqjyaihtldnetvkrw
zrfpagcpqfkwjimnrehibkctvwsyzocuikgpedchcyaotcamzaxpivq
kexrupgtmbmwwzlcpqccemtgvolpzqezybmgaedaganynsnjijfyvcn
```

You can use these seeds with the Qubic CLI to interact with your deployed smart contracts. For example:

```bash
./qubic-cli -nodeip YOUR_NODE_IP -nodeport YOUR_NODE_PORT -seed fwqatwliqyszxivzgtyyfllymopjimkyoreolgyflsnfpcytkhagqii -somecommand
```

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

## Frontend Application

### Frontend Overview
We've built a frontend application that demonstrates how to interact with the Qubic blockchain and specifically with the HM25 template smart contract deployed on the testnet. This application serves as an example of how to build user interfaces for Qubic-based dApps.

**Source Code:** [https://github.com/icyblob/hm25-frontend](https://github.com/icyblob/hm25-frontend) - Clone this repository to run the app locally, detailed setup instructions are provided in the repository README.

### Frontend Features
- Connect to a Qubic node through its RPC endpoint
- Integrated wallet support:
  - MetaMask Snap integration
  - WalletConnect support
  - Seed phrase login
  - Vault file authentication
- Interact with the HM25 template smart contract
  - Call Echo function (returns funds to the sender)
  - Call Burn function (burns funds permanently)
  - View contract statistics

### Connecting to a Node
By default, the app connects to a testnet node at `http://91.210.226.146`. 

**Important Notes**: 
1. The frontend app connects to a shared testnet node by default
2. Each team will be assigned their own dedicated node (accessible via SSH) where they'll deploy their custom contract
3. After running the deploy.sh script on your assigned node, you'll need to connect the frontend to your node
4. The RPC endpoint format has changed: Previously it was `http://<HOST_IP>:8000`, but now it's simply `http://<HOST_IP>` without the port

You can connect to your dedicated node:
1. Open the app in your browser
2. Click on the ConnectLink at the top right
3. Select "Connect to Server"
4. Enter your node URL (e.g., `http://your-node-ip`) without any port
5. Refresh the page

**How Transactions Work**:
The wallet is only used for signing transactions. After a transaction is signed, it will be broadcast to the network that the dapp is connecting to, as determined by the HTTP endpoint in the configuration.

The app interacts with the [HM25 template smart contract](https://github.com/qubic/core/blob/madrid-2025/src/contracts/HM25.h) that provides Echo, Burn, and GetStats functionality.

### Smart Contract Interaction Examples

The frontend interacts with the HM25 smart contract through several key functions defined in `src/components/api/HM25Api.jsx`. Here's how it works:

#### 1. Contract Constants

First, we define constants for interacting with the contract:

```javascript
export const HM25_CONTRACT_INDEX = 12  // The index of the HM25 contract on the testnet

// Function and procedure types
export const PROC_ECHO = 1     // Echo procedure type ID
export const PROC_BURN = 2     // Burn procedure type ID
export const FUNC_GET_STATS = 1  // GetStats function type ID
```

#### 2. Reading Contract State (GetStats)

To read data from the smart contract (like statistics), we use the `/v1/querySmartContract` endpoint:

```javascript
export async function fetchHM25Stats(httpEndpoint) {
    // Create the query data for the GetStats function (type 1)
    const queryData = makeJsonData(HM25_CONTRACT_INDEX, FUNC_GET_STATS, 0, '')
    
    try {
        // Make HTTP POST request to the node's RPC endpoint
        const response = await fetch(`${httpEndpoint}/v1/querySmartContract`, {
            method: 'POST',
            headers: HEADERS,
            body: JSON.stringify(queryData),
        })
        
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`)
        }
        
        const json = await response.json()
        // Decode the response data from base64
        const raw = base64.decode(json.responseData)
        const buf = Buffer.from(raw, 'binary')
        
        // Parse the 16-byte response (2 uint64 values)
        return {
            numberOfEchoCalls: buf.readBigUInt64LE(0),  // First 8 bytes
            numberOfBurnCalls: buf.readBigUInt64LE(8),  // Second 8 bytes
        }
    } catch (error) {
        console.error('Error fetching HM25 stats:', error)
        return {
            numberOfEchoCalls: 0n,
            numberOfBurnCalls: 0n,
        }
    }
}
```

#### 3. Creating Transaction for Echo Procedure

To call the Echo procedure (which returns funds to the sender), we build a transaction:

```javascript
export async function buildEchoTx(qHelper, sourcePublicKey, tick, amount) {
    const finalTick = tick + TICK_OFFSET  // Add offset to current tick
    const INPUT_SIZE = 0  // No input data for Echo
    const TX_SIZE = qHelper.TRANSACTION_SIZE + INPUT_SIZE
    const tx = new Uint8Array(TX_SIZE).fill(0)
    const dv = new DataView(tx.buffer)

    // Build the transaction
    let offset = 0
    // Source address
    tx.set(sourcePublicKey, offset)
    offset += qHelper.PUBLIC_KEY_LENGTH
    // Target contract (HM25)
    tx[offset] = HM25_CONTRACT_INDEX
    offset += qHelper.PUBLIC_KEY_LENGTH
    // Amount to send
    dv.setBigInt64(offset, BigInt(amount), true)
    offset += 8
    // Tick for the transaction
    dv.setUint32(offset, finalTick, true)
    offset += 4
    // Procedure type (Echo = 1)
    dv.setUint16(offset, PROC_ECHO, true)
    offset += 2
    // Input size
    dv.setUint16(offset, INPUT_SIZE, true)

    return tx
}
```

#### 4. Creating Transaction for Burn Procedure

Similarly, to call the Burn procedure (which permanently burns funds):

```javascript
export async function buildBurnTx(qHelper, sourcePublicKey, tick, amount) {
    const finalTick = tick + TICK_OFFSET
    const INPUT_SIZE = 0
    const TX_SIZE = qHelper.TRANSACTION_SIZE + INPUT_SIZE
    const tx = new Uint8Array(TX_SIZE).fill(0)
    const dv = new DataView(tx.buffer)

    // Build the transaction (similar structure to Echo but different procedure type)
    let offset = 0
    tx.set(sourcePublicKey, offset)
    offset += qHelper.PUBLIC_KEY_LENGTH
    tx[offset] = HM25_CONTRACT_INDEX
    offset += qHelper.PUBLIC_KEY_LENGTH
    dv.setBigInt64(offset, BigInt(amount), true)
    offset += 8
    dv.setUint32(offset, finalTick, true)
    offset += 4
    // Use PROC_BURN (2) instead of PROC_ECHO
    dv.setUint16(offset, PROC_BURN, true)
    offset += 2
    dv.setUint16(offset, INPUT_SIZE, true)

    return tx
}
```

#### 5. Complete Flow for Executing a Transaction

When a user interacts with the HM25 contract through the UI:

1. The frontend fetches the current tick from the node
2. It builds a transaction using `buildEchoTx` or `buildBurnTx`
3. The transaction is signed by the wallet
4. The signed transaction is broadcast to the node via the `/v1/broadcast-transaction` endpoint
5. The app shows the user the transaction status

This approach separates:
- The wallet (responsible only for signing transactions)
- The node connection (determined by the HTTP endpoint setting)
- The business logic (in the smart contract)

The frontend simply orchestrates these components to provide a seamless user experience.

## Project Structure
When you fork the Qubic core repository, you'll be working with this structure:

```
/
├── README.md                  # General information about Qubic core
├── src/                       # Source code directory
│   ├── contracts/             # Smart contract directory 
│   │   └── HM25.h             # Template smart contract file you'll modify
│   ├── contract_core/         # Core contract functionality
│   └── ...                    # Other Qubic core components
```

The key file you'll be modifying is `src/contracts/HM25.h`, which contains the template smart contract for the hackathon. **Remember that after making changes to this file, you must run the cleanup script and then redeploy to see your changes take effect.**

**On the testnet node** that we provide, you'll find the deployment scripts in this location:

```
/root/qubic/qubic-docker/      # Docker scripts directory on the testnet node
├── deploy.sh                  # Script to deploy your SC to a node
└── cleanup.sh                 # Script to clean up your deployment
```

These are the scripts you'll execute after connecting to the testnet node via SSH. **Remember that after making changes to your smart contract, you must run the cleanup script and then redeploy to see your changes take effect.**

## FAQs

**Q: Do I need to set up my own node?**  
A: No. We provide a test node that you can access via SSH. You'll be modifying the HM25.h template file on this node and deploying it using the provided scripts.

**Q: How do I develop my smart contract?**  
A: You should fork the Qubic core repository, switch to the madrid-2025 branch, and modify the HM25.h template file with your smart contract code. You can also modify files in the contract_core and contracts folders if needed, but this is not recommended for beginners.

**Q: How do I test my smart contract?**  
A: After developing your smart contract, contact us on the #dev channel on Discord. We'll provide you with SSH access to a test node where you can run the deployment script. After successful deployment, you can use the Qubic CLI to interact with your contract.

**Q: Can I update my smart contract after it's deployed?**  
A: Yes. Simply make your changes to the HM25.h file in your forked repository, and then run the deployment script again on the test node.

**Q: How do I know if my smart contract has issues?**  
A: When you run the deployment script, it will compile your code. If there are any issues, the deployment will fail, and the logs will provide information about what went wrong.

**Q: Can I build a frontend app that interacts with my smart contract?**  
A: Yes! After deploying your smart contract, you'll have access to an RPC endpoint that you can use to build a frontend application.

**Q: Do I have to use C++ for my smart contract?**  
A: Yes. At this time, Qubic smart contracts must be written in C++.

**Q: Will I get seeds/faucet for testing?**  
A: Yes! The testnet node is pre-configured with multiple pre-funded seeds, each containing 1 billion Qubic tokens. See the [Available Testnet Seeds](#available-testnet-seeds) section for the complete list of seeds you can use.

**Q: How do I verify my smart contract is working?**  
A: After deployment, use the Qubic CLI to send commands to your contract. For example:
```bash
./qubic-cli -nodeip YOUR_NODE_IP -nodeport YOUR_NODE_PORT -seed YOUR_SEED -somecommand
```

**Q: What is the IPO process for smart contracts in Qubic?**  
A: On the mainnet, each smart contract needs to be IPOed and approved by computors. This is a security mechanism to ensure the quality and integrity of smart contracts. For this hackathon, we've simplified the process by providing a pre-approved template contract (HM25.h) that you can modify. You can learn more about the IPO process at https://docs.qubic.org/learn/smart-contracts.

## Contact
If you have any questions or need assistance during the hackathon, please contact the team through the #dev channel on Discord.

---

*This repository and documentation were created for the Qubic Hackathon Madrid, March 22-23, 2024.*

package main

import (
	"context"
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"time"
)

const (
	MexcAddress   = "FDVORCTKJZVEBFYUXRVUHMPXLMADKSQKAOXLEXUASDGNXXGSXDIACIGHPYSF"
	GetBalanceUrl = "https://rpc.qubic.org/v1/balances/" + MexcAddress
)

type GetBalanceResponse struct {
	Balance struct {
		Id                         string `json:"id"`
		Balance                    string `json:"balance"`
		ValidForTick               int    `json:"validForTick"`
		LatestIncomingTransferTick int    `json:"latestIncomingTransferTick"`
		LatestOutgoingTransferTick int    `json:"latestOutgoingTransferTick"`
		IncomingAmount             string `json:"incomingAmount"`
		OutgoingAmount             string `json:"outgoingAmount"`
		NumberOfIncomingTransfers  int    `json:"numberOfIncomingTransfers"`
		NumberOfOutgoingTransfers  int    `json:"numberOfOutgoingTransfers"`
	} `json:"balance"`
}

func main() {
	err := run()
	if err != nil {
		log.Fatal(err)
	}
}

func run() error {
	log.Printf("MEXC Address: %s", MexcAddress)
	log.Printf("Get Balance URL: %s", GetBalanceUrl)

	ctx, cancel := context.WithTimeout(context.Background(), 5*time.Second)
	defer cancel()

	req, err := http.NewRequestWithContext(ctx, http.MethodGet, GetBalanceUrl, nil)
	if err != nil {
		return fmt.Errorf("creating new request: %w", err)
	}

	res, err := http.DefaultClient.Do(req)
	if err != nil {
		return fmt.Errorf("performing http request: %w", err)
	}
	defer res.Body.Close()

	if res.StatusCode != http.StatusOK {
		return fmt.Errorf("unexpected status code: %d", res.StatusCode)
	}

	var balanceResponse GetBalanceResponse
	if err := json.NewDecoder(res.Body).Decode(&balanceResponse); err != nil {
		return fmt.Errorf("decoding response: %w", err)
	}

	log.Printf("Balance Response for %s: %+v", MexcAddress, balanceResponse.Balance)
	log.Printf("Balance: %s", balanceResponse.Balance.Balance)

	//12303242050119
	//12303242050119

	return nil
}

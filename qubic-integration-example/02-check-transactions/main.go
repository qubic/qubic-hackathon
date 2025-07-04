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
	TickNumber             = "28211009"
	GetTickTransactionsUrl = "https://rpc.qubic.org/v2/ticks/" + TickNumber + "/transactions"
)

type GetTickTransactionsResponse struct {
	Transactions []struct {
		Transaction struct {
			SourceId     string `json:"sourceId"`
			DestId       string `json:"destId"`
			Amount       string `json:"amount"`
			TickNumber   int    `json:"tickNumber"`
			InputType    int    `json:"inputType"`
			InputSize    int    `json:"inputSize"`
			InputHex     string `json:"inputHex"`
			SignatureHex string `json:"signatureHex"`
			TxId         string `json:"txId"`
		} `json:"transaction"`
		Timestamp string `json:"timestamp"`
		MoneyFlew bool   `json:"moneyFlew"`
	} `json:"transactions"`
}

func main() {
	err := run()
	if err != nil {
		log.Fatal(err)
	}
}

func run() error {
	ctx, cancel := context.WithTimeout(context.Background(), 5*time.Second)
	defer cancel()

	req, err := http.NewRequestWithContext(ctx, http.MethodGet, GetTickTransactionsUrl, nil)
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

	var transactionsResponse GetTickTransactionsResponse
	err = json.NewDecoder(res.Body).Decode(&transactionsResponse)
	if err != nil {
		return fmt.Errorf("decoding response: %w", err)
	}

	//1985740152
	//1985740152

	log.Printf("Transactions for tick %s: %+v\n", TickNumber, transactionsResponse.Transactions)
	log.Printf("---------------------------------------------------\n")
	log.Printf("Total transactions: %d\n", len(transactionsResponse.Transactions))
	log.Printf("---------------------------------------------------\n")

	targetTxID := "pwatvfnmydxzqdmacacbdcmcfpdbtxytvihsfkblsfbqxbvljtyfegkfslll"

	for _, tx := range transactionsResponse.Transactions {
		if tx.Transaction.TxId != targetTxID {
			continue
		}

		log.Printf("Found transaction with ID %s: %+v\n", targetTxID, tx)
	}

	return nil
}

package main

import (
	"bytes"
	"context"
	"encoding/json"
	"fmt"
	"github.com/qubic/go-node-connector/types"
	"net/http"
	"time"
)

import "log"

const (
	BroadcastTransactionUrl = "https://rpc.qubic.org/v1/broadcast-transaction"
)

type BroadcastTransactionPayload struct {
	EncodedTransaction string `json:"encodedTransaction"`
}

type BroadcastTransactionResponse struct {
	PeersBroadcasted   int    `json:"peersBroadcasted"`
	EncodedTransaction string `json:"encodedTransaction"`
	TransactionId      string `json:"transactionId"`
}

// ZFXNTCRVPEUTHEMWRWWPMVEPNXLBHDNKMJGDTNCUQGDCCRJUEAHKIVNFGJKK -> FDVORCTKJZVEBFYUXRVUHMPXLMADKSQKAOXLEXUASDGNXXGSXDIACIGHPYSF
// create Transaction
// sign transaction
// encode transaction to base64
// send it to the broadcast-transaction endpoint

func main() {
	err := run()
	if err != nil {
		log.Fatal(err)
	}
}

func run() error {
	ctx, cancel := context.WithTimeout(context.Background(), 5*time.Second)
	defer cancel()

	sourceID := types.Identity("ZFXNTCRVPEUTHEMWRWWPMVEPNXLBHDNKMJGDTNCUQGDCCRJUEAHKIVNFGJKK")
	destID := types.Identity("FDVORCTKJZVEBFYUXRVUHMPXLMADKSQKAOXLEXUASDGNXXGSXDIACIGHPYSF")

	sourceIDPubKey, err := sourceID.ToPubKey(false)
	if err != nil {
		return fmt.Errorf("convert sourceID to pubkey: %w", err)
	}

	destIDPubKey, err := destID.ToPubKey(false)
	if err != nil {
		return fmt.Errorf("convert destID to pubkey: %w", err)
	}

	targetTick := 28210979 + 30

	tx := types.Transaction{
		SourcePublicKey:      sourceIDPubKey,
		DestinationPublicKey: destIDPubKey,
		Amount:               100,
		Tick:                 uint32(targetTick),
		InputType:            0,
	}

	err = tx.Sign("vifgmbvarewaclfbmgnxkrnijdmrjdksaglooamqfbxvfhystpqpnde")
	if err != nil {
		return fmt.Errorf("signing transaction: %w", err)
	}

	encodedTx, err := tx.EncodeToBase64()
	if err != nil {
		return fmt.Errorf("encoding transaction to base64: %w", err)
	}

	txID, err := tx.ID()
	if err != nil {
		return fmt.Errorf("getting transaction ID: %w", err)
	}

	log.Printf("Encoded Transaction: %s\n", encodedTx)
	log.Printf("Tx id: %s\n", txID)
	log.Printf("Target tick: %d\n", targetTick)

	payload := BroadcastTransactionPayload{
		EncodedTransaction: encodedTx,
	}

	buffer := &bytes.Buffer{}
	err = json.NewEncoder(buffer).Encode(payload)
	if err != nil {
		return fmt.Errorf("encoding payload to JSON: %w", err)
	}

	req, err := http.NewRequestWithContext(ctx, http.MethodPost, BroadcastTransactionUrl, buffer)
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

	var broadcastResponse BroadcastTransactionResponse
	err = json.NewDecoder(res.Body).Decode(&broadcastResponse)
	if err != nil {
		return fmt.Errorf("decoding response: %w", err)
	}

	log.Printf("Response: %+v\n", broadcastResponse)

	// Tx id: pwatvfnmydxzqdmacacbdcmcfpdbtxytvihsfkblsfbqxbvljtyfegkfslll
	// Target tick: 28211009

	return nil
}

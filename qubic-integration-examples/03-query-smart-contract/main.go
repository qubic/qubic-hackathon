package main

import (
	"bytes"
	"context"
	"encoding/base64"
	"encoding/binary"
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"time"
)

const (
	QuerySmartContractUrl = "https://rpc.qubic.org/v1/querySmartContract"
)

// https://github.com/qubic/core/blob/main/src/contracts/Qearn.h

/*
struct getLockInfoPerEpoch_input {
		uint32 Epoch;
};

struct getLockInfoPerEpoch_output {
uint64 lockedAmount;
uint64 bonusAmount;
uint64 currentLockedAmount;
uint64 currentBonusAmount;
uint64 yield;
};
*/

type GetLockInfoPerEpochInput struct {
	Epoch uint32
}

type GetLockInfoPerEpochOutput struct {
	LockedAmount        uint64 `json:"lockedAmount"`
	BonusAmount         uint64 `json:"bonusAmount"`
	CurrentLockedAmount uint64 `json:"currentLockedAmount"`
	CurrentBonusAmount  uint64 `json:"currentBonusAmount"`
	Yield               uint64 `json:"yield"`
}

type QuerySmartContractRequestPayload struct {
	ContractIndex int    `json:"contractIndex"` // 9
	InputType     int    `json:"inputType"`     // 1
	InputSize     int    `json:"inputSize"`     // 4
	RequestData   string `json:"requestData"`   //base64 encoded
}

type QuerySmartContractResponse struct {
	ResponseData string `json:"responseData"` // base64 encoded
}

//REGISTER_USER_FUNCTION(getLockInfoPerEpoch, 1);

func main() {
	err := run()
	if err != nil {
		log.Fatal(err)
	}
}

func run() error {
	ctx, cancel := context.WithTimeout(context.Background(), 5*time.Second)
	defer cancel()

	input := GetLockInfoPerEpochInput{
		Epoch: 167,
	}

	inputBytes := make([]byte, 4)
	_, err := binary.Encode(inputBytes, binary.LittleEndian, input)
	if err != nil {
		return fmt.Errorf("encoding input to binary: %w", err)
	}

	fmt.Printf("Input size: %d\n", len(inputBytes))

	requestData := base64.StdEncoding.EncodeToString(inputBytes)
	inputSize := 4
	inputType := 1
	contractIndex := 9
	payload := QuerySmartContractRequestPayload{
		ContractIndex: contractIndex,
		InputType:     inputType,
		InputSize:     inputSize,
		RequestData:   requestData,
	}

	buffer := &bytes.Buffer{}
	err = json.NewEncoder(buffer).Encode(payload)
	if err != nil {
		return fmt.Errorf("encoding payload to JSON: %w", err)
	}

	req, err := http.NewRequestWithContext(ctx, http.MethodPost, QuerySmartContractUrl, buffer)
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

	var querySmartContractResponse QuerySmartContractResponse
	err = json.NewDecoder(res.Body).Decode(&querySmartContractResponse)
	if err != nil {
		return fmt.Errorf("decoding response: %w", err)
	}

	responseData, err := base64.StdEncoding.DecodeString(querySmartContractResponse.ResponseData)
	if err != nil {
		return fmt.Errorf("decoding response data from base64: %w", err)
	}

	var output GetLockInfoPerEpochOutput
	err = binary.Read(bytes.NewReader(responseData), binary.LittleEndian, &output)
	if err != nil {
		return fmt.Errorf("decoding response data to output struct: %w", err)
	}

	log.Printf("Output: %+v\n", output)

	return nil
}

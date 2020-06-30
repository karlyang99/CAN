package main

import (
	"encoding/csv"
	"fmt"
	"os"
	"path/filepath"
)

func main() {
	p, err := filepath.Abs(`C:\Users\Cedar\Documents\GitHub\CAN\1.csv`)
	records, err := readCSV(p)
	for i := 0; i < len(records); i++ {
		fmt.Println(records[i])
	}
	if err != nil {
	}
}

func readCSV(path string) ([][]string, error) {
	csvfile, err := os.Open(path)
	if err != nil {
		return nil, err
	}

	defer csvfile.Close()

	reader := csv.NewReader(csvfile)
	fields, err := reader.ReadAll()

	return fields, nil
}

package main

import (
	"encoding/csv"
	"fmt"
	"os"
	"strconv"
	/*"path/filepath"*/)

func main() {
	p := "one.csv"
	records, err := readCSV(p)
	for i := 0; i < len(records); i++ {
		fmt.Println(records[i])
		a, errr := strconv.Atoi(records[i][2])
		aa := uint32(a)
		b, errr := strconv.Atoi(records[i][3])
		byteB := byte(b)
		c, errr := strconv.Atoi(records[i][4])
		byteC := byte(c)
		d, errr := strconv.Atoi(records[i][5])
		byteD := byte(d)
		if errr != nil {
		}
		influxdbwrite(aa, byteB, byteC, byteD)
	}
	if err != nil {
	}
	fmt.Println("hi")
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

func influxdbwrite(alpha uint32, beta byte, charlie byte, delta byte) {

}

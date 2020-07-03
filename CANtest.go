package main

import (
	"encoding/csv"
	"fmt"
	"os"
	"strconv"
		_ "github.com/influxdata/influxdb1-client"
	client "github.com/influxdata/influxdb1-client/v2"
	/*"path/filepath"*/)

func main() {
	p := "four.csv"
	records, err := readCSV(p)
	for i := 0; i < /*len(records)*/ 10; i++ {
		fmt.Println(records[i])
		current, power, voltage := 0, 0, 0
		ids, errr := strconv.ParseInt(records[i][1], 16, 64)
		id := uint32(ids)
		//b, errr := strconv.ParseInt(records[i][4], 16, 64)
		c, errr := strconv.ParseInt(records[i][5]+records[i][4], 16, 64)
		current = (c)/100
		//d, errr := strconv.ParseInt(records[i][6], 16, 64)
		e, errr := strconv.ParseInt(records[i][7]+records[i][6], 16, 64)
		power = (e)/10
		//f, errr := strconv.ParseInt(records[i][8], 16, 64)
		g, errr := strconv.ParseInt(records[i][9]+records[i][8], 16, 64)
		voltage = (g)/100
		if errr != nil {
		}
		influxdbwrite(id, current, power, voltage)
		fmt.Println(current, voltage, power)
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

func influxdbwrite(id uint32, current  int, power  int, voltage int) {
	c, err := client.NewHTTPClient(client.HTTPConfig{
		Addr: "http://localhost:8086",
	})

	if err != nil {
		fmt.Println("Error creating InfluxDB Client: ", err.Error())
	}
	defer c.Close()

	// Create a new point batch
	bp, _ := client.NewBatchPoints(client.BatchPointsConfig{
		Database: "Testing1",
		// Precision: "s",
	})

	//Create a point and add to batch
	tags := map[string]string{"battery": "176"} // 0D0
	fields := map[string]interface{}{
		"current":     	10,
		"power":	20,
		"voltage": 	30,
	}
	
// fmt.Println(id, tags,fields)
	tags["battery"] = strconv.FormatUint(uint64(id), 10)
	fields["current"] = current
	fields["power"] = power
	fields["voltage"] = voltage

	// pt, err := client.NewPoint("battery", tags, fields, time.Now())
	pt, err := client.NewPoint("battery", tags, fields)
	if err != nil {
		fmt.Println("Error: ", err.Error())
	}
	bp.AddPoint(pt)

	// Write the batch
	c.Write(bp)
}

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
	p := "one.csv"
	records, err := readCSV(p)
	for i := 0; i < 10; i++ {
		fmt.Println(records[i])
		current, power, voltage := 0, 0, 0
		ids, errr := strconv.Atoi(records[i][1])
		id := uint32(ids)
		b, errr := strconv.Atoi(records[i][4])
		c, errr := strconv.Atoi(records[i][5])
		current = ((c/10)*4096 + (c%10)*256 + (b/10)*16 + (b%10))/100.0
		d, errr := strconv.Atoi(records[i][6])
		e, errr := strconv.Atoi(records[i][7])
		power = ((e/10)*4096 + (e%10)*256 + (d/10)*16 + (d%10))/10.0
		f, errr := strconv.Atoi(records[i][8])
		g, errr := strconv.Atoi(records[i][9])
		voltage = ((g/10)*4096 + (g%10)*256 + (f/10)*16 + (f%10))/100.0
		if errr != nil {
		}
		influxdbwrite(id, current, power, voltage)
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
		Database: "Testing2",
		// Precision: "s",
	})

	/* Create a point and add to batch
	tags := map[string]string{"battery": "176"} // 0D0
	fields := map[string]interface{}{
		"current":     10,
		"voltage":     20,
		"tempareture": 30,
	}*/
	
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

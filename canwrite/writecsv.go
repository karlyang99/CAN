//read in csv directly to influxdb
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
	p := "two.csv"
	records, err := readCSV(p)
	for i := 0; i < len(records); i++ {
		fmt.Println(records[i])
		ids, errr := strconv.ParseInt(records[i][1], 16, 64)
		id := uint32(ids)
		c, errr := strconv.ParseInt(records[i][5]+records[i][4], 16, 64)
		fc := float64(c)
		current := (fc)/1000.0
		e, errr := strconv.ParseInt(records[i][7]+records[i][6], 16, 64)
		power := (e)/100
		g, errr := strconv.ParseInt(records[i][9]+records[i][8], 16, 64)
		voltage := (g)/100
		if errr != nil {
		}
		influxdbwrite(id, current, power, voltage)
	}
	if err != nil {
	}
	fmt.Println("Finished!")
}

func readCSV(path string) ([][]string, error) {
	//convert a csv file into 2d string slice (Requires a rectangular data set)
	csvfile, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer csvfile.Close()
	reader := csv.NewReader(csvfile)
	fields, err := reader.ReadAll()
	return fields, nil
}

func influxdbwrite(id uint32, current  float64, power  int64, voltage int64) {
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

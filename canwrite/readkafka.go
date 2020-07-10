//Read in a kafkamessage and write it into influxDB
package main

import(
        "fmt"
        //"github.com/go-daq/canbus"
        "github.com/segmentio/kafka-go"
        "context"
        //"encoding/csv"
        //"os"
        "strconv"
        _ "github.com/influxdata/influxdb1-client"
        client "github.com/influxdata/influxdb1-client/v2"

)

func main(){
	//create kafkareader
	r := kafka.NewReader(kafka.ReaderConfig{
	    Brokers:   []string{"localhost:9092"},
	    Topic:     "Topic-1",
	    Partition: 0,
	    MinBytes:  0, // 10KB
	    MaxBytes:  10e6, // 10MB
	})
	r.SetOffset(42)
	//hang kafkareader
	for {
		//
		m, err := r.ReadMessage(context.Background())
		if err != nil {
			break
		}
		msg := string(m.Value)
		i, err := strconv.ParseInt(msg,10,32)
		fcurrent := float64(i)/1000000.0
		voltage := i%100
		power := (i%10000)/100
		var id uint32
		id = 8344
		influxdbwrite(id, fcurrent, power, voltage)
		fmt.Println(msg)
	}
	r.Close()
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
			Database: "Testing1",
			// Precision: "s",
	})
	//Create a point and add to batch
	tags := map[string]string{"battery": "176"} // 0D0
	fields := map[string]interface{}{
			"current":      10,
			"power":        20,
			"voltage":      30,
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



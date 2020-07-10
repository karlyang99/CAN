//Convert a string into a kafka message and send
func kafkawrite(data string){
	w := kafka.NewWriter(kafka.WriterConfig{
    		Brokers: []string{"localhost:9092"},
    		Topic:   "Topic-1",
	})

	w.WriteMessages(context.Background(),
    		kafka.Message{
        		Key:   []byte("Key-A"),
        		Value: []byte(data),
    		},
	)
	fmt.Println("message written to kafka:" + data)
}

//Read out a kafka message and return data as a string
func kafkaread()(string){
	r := kafka.NewReader(kafka.ReaderConfig{
		Brokers:   []string{"localhost:9092"},
		Topic:     "Topic-1",
		Partition: 0,
		MinBytes:  0,
		MaxBytes:  10e6, // 10MB
	}) //create and configure kafkareader
	r.SetOffset(42)
	m, err := r.ReadMessage(context.Background()) //hang kafkareader
	if err != nil {
		break
	}
	msg := string(m.Value)
	return msg
	r.Close()
}

//Write a string into CAN-BUS
func writeCanBus(data string){
	sck, err := canbus.New()
	err = sck.Bind("can0")
	if err != nil{
			fmt.Println("theres an error")
	}
	by := []byte(data)
	sck.Send(id, by)
}

//Read a string out of CAN-BUS
func readCanBus()(string){
	sck, err := canbus.New()
	err = sck.Bind("can0")
	if err != nil{
			fmt.Println("Error in creating socket")
	}
	for{
	id, data, err := sck.Recv()
	if err != nil{
		fmt.Println("Error in socket hanging/recieving")
	}
	return data
}
	

//Write various data values into InfluxDB (currently supported: id, current, power, voltage)
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
	})

	//Create a point and add to batch
	tags := map[string]string{"battery": "176"} // 0D0
	fields := map[string]interface{}{
			"current":      10,
			"power":        20,
			"voltage":      30,
	}
	//Update points to data passed
	tags["battery"] = strconv.FormatUint(uint64(id), 10)
	fields["current"] = current
	fields["power"] = power
	fields["voltage"] = voltage

	//Catch Errors
	pt, err := client.NewPoint("battery", tags, fields)
	if err != nil {
			fmt.Println("Error: ", err.Error())
	}
	bp.AddPoint(pt)

	// Write the batch into database
	c.Write(bp)
}

//Convert a csv file (passed in pathname) into a 2d string slice. Requires a rectangular set of data
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

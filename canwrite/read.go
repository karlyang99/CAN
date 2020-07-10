//Read a message from CANBUS connection, write message into kafka
package main

import(
	"fmt"
	"github.com/go-daq/canbus"
	"github.com/segmentio/kafka-go"
	"context"
)

func main(){
	//Create canbus socket to receive messages
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
		//print out recieved message from CAN-bus
		str := fmt.Sprint("ID:",id , " Data:",data)
		fmt.Println(str)
		//create go sub process for individual string so loop does not hang
		go func(){
			s := string(data)
			kafkawrite(s)
		}()
	}
}


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

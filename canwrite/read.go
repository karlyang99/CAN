package main

import(
	"fmt"
	"github.com/go-daq/canbus"
	"github.com/segmentio/kafka-go"
	"context"
)

func main(){
	sck, err := canbus.New()
        err = sck.Bind("can0")
        if err != nil{
                fmt.Println("theres an error")
        }
	for{
		id, data, er := sck.Recv()
		if er != nil{
			fmt.Println("error in pt2")
		}
		str := fmt.Sprint("ID:",id , " Data:",data)
		fmt.Println(str)
		go func(){
			s := string(data)
			kafkawrite(s)
		}()
	}
	fmt.Println("go away")
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

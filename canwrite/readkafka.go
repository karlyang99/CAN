package main

import(
	"fmt"
        //"github.com/go-daq/canbus"
        "github.com/segmentio/kafka-go"
        "context"
)

func main(){
r := kafka.NewReader(kafka.ReaderConfig{
    Brokers:   []string{"localhost:9092"},
    Topic:     "topic-1",
    Partition: 0,
    MinBytes:  0, // 10KB
    MaxBytes:  10e6, // 10MB
})
r.SetOffset(42)

fmt.Println("???")

for {
	fmt.Println("working 1")
    m, err := r.ReadMessage(context.Background())
fmt.Println("working 2")
fmt.Println(m)

 if err != nil {
        break
    	fmt.Println("kafka failed due to error.  Error:")
	}
    fmt.Printf("message at offset %d: %s = %s\n", m.Offset, string(m.Key), string(m.Value))
}

r.Close()
}

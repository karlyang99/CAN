 
package main

import(
        "github.com/go-daq/canbus"
        "fmt"
        "time"
        "strconv"
        "math/rand"
)

func main(){
        for{
                sendBy()
                time.Sleep(1 * time.Second)
        }
}

func sendBy(){
        sck, err := canbus.New()
        err = sck.Bind("can0")
        if err != nil{
                fmt.Println("theres an error")
        }
        id := rand.Uint32()
        data := rand.Int63n(89999999)+10000000
        str := strconv.FormatInt(data,10)
        by := []byte(str)
        sck.Send(id, by)
}

package main

import (
	"fmt"
	"io/ioutil"
	"log"
	"time"

	"github.com/openlab-aux/golableuchtung"

	serial "github.com/huin/goserial"
)

func main() {

	leucht := lableuchtung.LabLeucht{}

	c := &serial.Config{
		Name: "/dev/ttyACM0",
		Baud: 115200,
	}

	var err error

	leucht.ReadWriteCloser, err = serial.OpenPort(c)
	if err != nil {
		log.Fatal(err)
	}
	leucht.ResponseTimeout = 10 * time.Millisecond

	time.Sleep(3 * time.Second)
	ioutil.ReadAll(leucht) // remove fnord

	fmt.Println("testing RED")
	if err := leucht.SendPackage(lableuchtung.Package{0, 255, 0, 0}); err != nil {
		fmt.Println(err)
	}
	time.Sleep(1 * time.Second)
	fmt.Println("testing GREEN")
	if err := leucht.SendPackage(lableuchtung.Package{0, 0, 255, 0}); err != nil {
		fmt.Println(err)
	}
	time.Sleep(1 * time.Second)
	fmt.Println("testing BLUE")
	if err := leucht.SendPackage(lableuchtung.Package{0, 0, 0, 255}); err != nil {
		fmt.Println(err)
	}
	time.Sleep(1 * time.Second)
	fmt.Println("testing BEACON")
	if err := leucht.SendPackage(lableuchtung.EnableBeacon); err != nil {
		fmt.Println(err)
	}
	time.Sleep(500 * time.Millisecond)
	if err := leucht.SendPackage(lableuchtung.DisableBeacon); err != nil {
		fmt.Println(err)
	}
	time.Sleep(1 * time.Second)

	fmt.Println("testing fade --> red")
	if err := leucht.SendPackage(lableuchtung.Package{10, 255, 0, 0}); err != nil {
		fmt.Println(err)
	}
	time.Sleep(1 * time.Second)
	fmt.Println("testing fade --> green")
	if err := leucht.SendPackage(lableuchtung.Package{10, 0, 255, 0}); err != nil {
		fmt.Println(err)
	}
	time.Sleep(1 * time.Second)
	fmt.Println("testing fade --> blue")
	if err := leucht.SendPackage(lableuchtung.Package{10, 0, 0, 255}); err != nil {
		fmt.Println(err)
	}

	time.Sleep(1 * time.Second)
	fmt.Println("starting penetration test")
	for {

		for i := 0; i < 255; i++ {
			if err := leucht.SendPackage(lableuchtung.Package{0, byte(255 - i), byte(i), 0}); err != nil {
				fmt.Println(err)
			}
		}

		for i := 0; i < 255; i++ {
			if err := leucht.SendPackage(lableuchtung.Package{0, 0, byte(255 - i), byte(i)}); err != nil {
				fmt.Println(err)
			}
		}

		for i := 0; i < 255; i++ {
			if err := leucht.SendPackage(lableuchtung.Package{0, byte(i), 0, byte(255 - i)}); err != nil {
				fmt.Println(err)
			}
		}

	}

}

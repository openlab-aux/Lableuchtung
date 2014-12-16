package main

import (
	"errors"
	"fmt"
	"io"
	"log"
	"time"

	serial "github.com/huin/goserial"
)

type Package []byte

func (p Package) checkSum() byte {
	return p[0] ^ p[1] ^ p[2] ^ p[3]
}

var (
	EnableAutomode = Package{251, 0, 0, 0}
	EnableBeacon   = Package{252, 0, 0, 0}
	DisableBeacon  = Package{253, 0, 0, 0}
)

type LabLeucht struct {
	io.ReadWriteCloser
}

func (l *LabLeucht) sendPackage(pkg Package) error {
	buf := make([]byte, 1)

	fmt.Printf("Sending package [%3d %3d %3d %3d]\n", pkg[0], pkg[1], pkg[2], pkg[3])

	_, err := l.Write([]byte(pkg))
	if err != nil {
		return err
	}

	//TODO use go routine and timeout
	for {
		n, err := l.Read(buf)
		if err != nil && err != io.EOF {
			return err
		}
		if n != 0 {
			break
		}
	}

	if buf[0] != pkg.checkSum() {
		return errors.New("bad response: got " + fmt.Sprint(buf[0]) + ", want " + fmt.Sprint(pkg.checkSum()))
	}

	return nil
}

func main() {

	leucht := LabLeucht{}

	c := &serial.Config{
		Name: "/dev/ttyACM0",
		Baud: 115200,
	}

	var err error

	leucht.ReadWriteCloser, err = serial.OpenPort(c)
	if err != nil {
		log.Fatal(err)
	}

	time.Sleep(1 * time.Second)

	fmt.Println("testing RED")
	if err := leucht.sendPackage([]byte{254, 255, 0, 0}); err != nil {
		fmt.Println(err)
	}
	time.Sleep(1 * time.Second)
	fmt.Println("testing GREEN")
	if err := leucht.sendPackage([]byte{254, 0, 255, 0}); err != nil {
		fmt.Println(err)
	}
	time.Sleep(1 * time.Second)
	fmt.Println("testing BLUE")
	if err := leucht.sendPackage([]byte{254, 0, 0, 255}); err != nil {
		fmt.Println(err)
	}
	time.Sleep(1 * time.Second)
	fmt.Println("testing BEACON")
	if err := leucht.sendPackage(EnableBeacon); err != nil {
		fmt.Println(err)
	}
	time.Sleep(1 * time.Second)
	if err := leucht.sendPackage(DisableBeacon); err != nil {
		fmt.Println(err)
	}
	time.Sleep(1 * time.Second)

	for {

		var i byte

		for i = 0; i < 254; i++ {
			//time.Sleep(1 * time.Microsecond)
			if err := leucht.sendPackage([]byte{254, i, byte(255 - i), 0}); err != nil {
				fmt.Println(err)
			}
		}

		for i = 255; i > 1; i-- {
			//time.Sleep(1 * time.Microsecond)
			if err := leucht.sendPackage([]byte{254, i, byte(255 - i), 0}); err != nil {
				fmt.Println(err)
			}
		}

	}
}

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
	responseTimeout time.Duration
}

func (l *LabLeucht) sendPackage(pkg Package) error {

	fmt.Printf("Sending package [%3d %3d %3d %3d]\n", pkg[0], pkg[1], pkg[2], pkg[3])

	var timeoutMult time.Duration

	if pkg[0] < 251 {
		timeoutMult = time.Duration(pkg[0]) * 10
	}

	timeout := time.After(time.Millisecond*timeoutMult + l.responseTimeout)

	_, err := l.Write([]byte(pkg))
	if err != nil {
		return err
	}

	rspCh := make(chan byte)
	errCh := make(chan error)

	go func() {
		buf := make([]byte, 1)
		for {
			n, err := l.Read(buf)
			if err != nil && err != io.EOF {
				errCh <- err
				return
			}
			if n != 0 {
				rspCh <- buf[0]
				return
			}
		}
	}()

	select {
	case err := <-errCh:
		return err

	case r := <-rspCh:
		if r != pkg.checkSum() {
			return errors.New("bad response: got " + fmt.Sprint(r) + ", want " + fmt.Sprint(pkg.checkSum()))
		}

	case <-timeout:
		return errors.New("response timed out")
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
	leucht.responseTimeout = 1 * time.Millisecond

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

		for i := 0; i < 255; i++ {
			//time.Sleep(1 * time.Microsecond)
			if err := leucht.sendPackage([]byte{254, byte(255 - i), byte(i), 0}); err != nil {
				fmt.Println(err)
			}
		}

		for i := 0; i < 255; i++ {
			//time.Sleep(1 * time.Microsecond)
			if err := leucht.sendPackage([]byte{254, 0, byte(255 - i), byte(i)}); err != nil {
				fmt.Println(err)
			}
		}

		for i := 0; i < 255; i++ {
			//time.Sleep(1 * time.Microsecond)
			if err := leucht.sendPackage([]byte{254, byte(i), 0, byte(255 - i)}); err != nil {
				fmt.Println(err)
			}
		}

	}
}

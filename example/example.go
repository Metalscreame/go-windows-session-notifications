package main

import (
	"github.com/go-windows-session-notifications"
	"log"
	"os"
)



func main() {
	f, err := os.OpenFile("log.txt", os.O_RDWR | os.O_CREATE | os.O_APPEND, 0666)
	if err != nil {
		log.Fatalf("error opening file: %v", err)
	}
	defer f.Close()

	log.SetOutput(f)
	quit := make(chan int)

	chanMessages := make(chan session_notifications.Message, 100)
	chanClose := make(chan int)

	go func() {
		for {
			select {
			case m := <-chanMessages:
				switch m.UMsg {
				case session_notifications.WM_WTSSESSION_CHANGE:
					switch m.Param {
					case session_notifications.WTS_SESSION_LOCK:
						log.Println("session locked")
					case session_notifications.WTS_SESSION_UNLOCK:
						log.Println("session unlocked")
					case session_notifications.WTS_SESSION_LOGOFF:
						log.Println("session logged off")
					}
				case session_notifications.WM_QUERYENDSESSION:
					log.Println("log off or shutdown")
					switch m.Param {
					case session_notifications.ENDSESSION_LOGOFF:
						log.Println("log off")
					case session_notifications.ENDSESSION_SHUTDOWN:
						log.Println("shutdown")
					default:
						log.Println("default, ", m.Param)
					}
				}
				close(m.ChanOk)
			}
		}
	}()

	session_notifications.Subscribe(chanMessages, chanClose)

	// ctrl+c to quit
	<-quit
}

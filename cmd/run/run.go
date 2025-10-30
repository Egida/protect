package main

import (
	"fmt"
	"log"
	"os"
	"os/exec"
	"os/signal"
	"runtime"
	"sync"
	"syscall"
	"time"

	"github.com/joho/godotenv"
	"github.com/spacegame-llc/space/modules/api"
	"github.com/spacegame-llc/space/modules/common"
	"github.com/spacegame-llc/space/modules/envvar"
)

var cmd *exec.Cmd

func bash(command string) {

	cmd = exec.Command("bash", "-c", command)
	if cmd == nil {
		fmt.Printf("error: could not run bash!\n")
		os.Exit(1)
	}

	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stdout

	cmd.Env = os.Environ()
	cmd.Env = append(cmd.Env, "LD_LIBRARY_PATH=.")

	c := make(chan os.Signal, 1)
	signal.Notify(c, os.Interrupt, syscall.SIGTERM)
	go func() {
		sig := <-c
		if cmd.Process != nil {
			fmt.Printf("\n\n")
			if err := cmd.Process.Signal(sig); err != nil {
				fmt.Printf("error trying to signal child process: %v\n", err)
			}
			cmd.Wait()
		}
		os.Exit(1)
	}()

	if err := cmd.Run(); err != nil {
		fmt.Printf("error: failed to run command: %v\n", err)
		os.Exit(1)
	}

	cmd.Wait()
}

func bash_ignore_result(command string) {

	cmd = exec.Command("bash", "-c", command)
	if cmd == nil {
		fmt.Printf("error: could not run bash!\n")
		os.Exit(1)
	}

	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stdout

	cmd.Run()

	cmd.Wait()
}

func bash_no_wait(command string) {

	cmd = exec.Command("bash", "-c", command)
	if cmd == nil {
		fmt.Printf("error: could not run bash!\n")
		os.Exit(1)
	}

	cmd.Run()
}

func main() {

	args := os.Args

	if len(args) < 2 || (len(args) == 2 && args[1] == "help") {
		help()
		return
	}

	err := godotenv.Load(".env")
	if err != nil {
		fmt.Printf("\nCould not load .env file. Please run 'space env local'\n\n")
		os.Exit(1)
	}

	command := args[1]

	if command == "test" || command == "tests" {
		test()
	} else if command == "backend" {
		backend(args)
	} else if command == "game" {
		game()
	} else if command == "client" {
		client()
	} else if command == "server" {
		server()
	} else if command == "happy-path" {
		happy_path()
	} else if command == "happy-path-no-wait" {
		happy_path_no_wait()
	} else if command == "update-schemas" {
		update_schemas()
	} else if command == "update-config" {
		update_config()
	} else if command == "upload-config" {
		upload_config()
	} else if command == "functional-test" || command == "func-test" || command == "func-tests" {
		functional_test(args[2:])
	} else {
		fmt.Printf("\nunknown command\n\n")
	}
}

func help() {
	fmt.Printf("\nsyntax:\n\n    run <action> [args]\n\n")
}

func test() {
	bash("cd dist && ./test")
	bash("go test ./...")
	fmt.Printf("\n")
}

func backend(args []string) {
	fmt.Printf("\n")
	if len(args) >= 3 {
		bash(fmt.Sprintf("HTTP_PORT=50000 SPACE_GAME_LEVEL=%s SPACE_GAME_CONFIG_PATH=game/Config.bin go run ./cmd/backend/backend.go", args[2]))
	} else {
		bash("HTTP_PORT=50000 SPACE_GAME_CONFIG_PATH=game/Config.bin go run ./cmd/backend/backend.go")
	}
	fmt.Printf("\n")
}

func game() {
	levelName := envvar.GetString("SPACE_GAME_LEVEL", "Test")
	fmt.Printf("\n")
	bash(fmt.Sprintf("cd dist && ./launcher listen %s", levelName))
	fmt.Printf("\n")
}

func server() {
	levelName := envvar.GetString("SPACE_GAME_LEVEL", "Test")
	serverAddress := envvar.GetString("SPACE_GAME_SERVER_ADDRESS", "127.0.0.1:40000")
	fmt.Printf("\n")
	os := runtime.GOOS
	if os == "linux" {
		// IMPORTANT: watch out for NUMA bullshit on threadrippers!
		bash(fmt.Sprintf("cd dist && taskset -c 0-7 ./launcher dedicated %s %s", levelName, serverAddress))
	} else {
		bash(fmt.Sprintf("cd dist && ./launcher dedicated %s %s", levelName, serverAddress))
	}
	fmt.Printf("\n")
}

func happy_path() {
	fmt.Printf("\n")
	bash("./dist/happy_path")
	fmt.Printf("\n")
}

func happy_path_no_wait() {
	fmt.Printf("\n")
	bash("./dist/happy_path true")
	fmt.Printf("\n")
}

func update_schemas() {
	fmt.Printf("\n")
	bash("./dist/update_schemas")
	fmt.Printf("\n")
}

func update_config() {
	fmt.Printf("\n")
	bash("./dist/update_config")
	fmt.Printf("\n")
}

func upload_config() {
	fmt.Printf("\n")
	bash("./dist/upload_config")
	fmt.Printf("\n")
}

func functional_test(tests []string) {
	command := "./dist/functional_tests"
	if len(tests) > 0 {
		for _, test := range tests {
			bash(fmt.Sprintf(" %s %s", command, test))
		}
	} else {
		bash(command)
	}
}

func client() {

	backendToken := ""

	refreshTokenRate := 1000

	env := envvar.GetString("SPACE_GAME_ENV", "local")
	backendURL := envvar.GetString("SPACE_GAME_BACKEND_URL", "http:127.0.0.1")
	backendPort := envvar.GetString("SPACE_GAME_BACKEND_PORT", "50000")

	userHash := uint64(0)

	// client init
	{
		var request api.ClientInitRequest
		var response api.ClientInitResponse

		if env != "local" {
			homeDir, err := os.UserHomeDir()
			if err != nil {
				log.Fatalf("could not get user home dir: %v", err)
			}
			authToken, err := os.ReadFile(fmt.Sprintf("%s/space-secrets/dev.txt", homeDir))
			if err != nil {
				log.Fatalf("could not load dev token: %v", err)
			}
			request.Platform = "dev"
			request.AuthToken = string(authToken)
		}

		err := common.PutJSON(fmt.Sprintf("%s:%s/client_init", backendURL, backendPort), &request, &response)
		if err != nil {
			log.Fatalf("could not post client_init: %v", err)
		}

		backendToken = response.BackendToken

		refreshTokenRate = response.RefreshTokenRate

		userHash = response.UserHash
	}

	// start a client connect
	{
		var request api.ClientConnectRequest
		var response api.ClientConnectResponse

		request.BackendToken = backendToken

		err := common.PutJSON(fmt.Sprintf("%s:%s/client_connect", backendURL, backendPort), &request, &response)
		if err != nil {
			log.Fatalf("could not post client_connect: %v", err)
		}
	}

	// refresh backend token periodically

	var mutex sync.Mutex

	go func() {

		ticker := time.NewTicker(time.Duration(refreshTokenRate) * time.Second)
		for {
			select {
			case _ = <-ticker.C:
				{
					var request api.ClientRefreshTokenRequest
					var response api.ClientRefreshTokenResponse

					mutex.Lock()
					request.BackendToken = backendToken
					mutex.Unlock()

					err := common.PutJSON(fmt.Sprintf("%s:%s/client_refresh_token", backendURL, backendPort), &request, &response)
					if err != nil {
						log.Fatalf("could not post client_refresh_token: %v", err)
					}

					if response.BackendToken == "" || response.BackendToken == request.BackendToken {
						log.Fatalf("invalid backend token")
					}

					mutex.Lock()
					backendToken = response.BackendToken
					mutex.Unlock()
				}
			}
		}

	}()

	// run client update once per-second until we are ready to join

	var level string
	var serverAddress string
	var connectToken string

	for {

		time.Sleep(time.Second)

		var request api.ClientUpdateRequest
		var response api.ClientUpdateResponse

		mutex.Lock()
		request.BackendToken = backendToken
		mutex.Unlock()

		err := common.PutJSON(fmt.Sprintf("%s:%s/client_update", backendURL, backendPort), &request, &response)
		if err != nil {
			log.Fatalf("could not post client_update: %v", err)
		}

		if response.Ready {
			level = response.Level
			serverAddress = response.ServerAddress
			connectToken = response.ConnectToken
			break
		} else {
			fmt.Printf("servers are full. join queue position is %d/%d\n", response.JoinQueueIndex+1, response.JoinQueueSize)
		}
	}

	// run client update in the background once per-second while we play

	go func() {

		ticker := time.NewTicker(time.Second)
		for {
			select {
			case _ = <-ticker.C:
				{
					var request api.ClientUpdateRequest
					var response api.ClientUpdateResponse

					mutex.Lock()
					request.BackendToken = backendToken
					mutex.Unlock()

					err := common.PutJSON(fmt.Sprintf("%s:%s/client_update", backendURL, backendPort), &request, &response)
					if err != nil {
						log.Fatalf("could not post client_update: %v", err)
					}
				}
			}
		}

	}()

	bash(fmt.Sprintf("cd dist && ./launcher client %x %s %s %s", userHash, level, serverAddress, connectToken))

	fmt.Printf("\n")
}

// ---------------------------------------------------------------

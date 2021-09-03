const TCPServer = TCP();
const tcpServer = new TCPServer('127.0.0.1', 8989);
tcpServer.socket();
tcpServer.setReusePort(1);
tcpServer.bind();
tcpServer.listen();
const isMaster = Child_Process.getEnv("isMaster") === "";
for (let i = 0; i < 3; i++) {
    if (Child_Process.fork() === 0) {
        while(1) {
            tcpServer.accept();
        } 
    }
}
Child_Process.wait();


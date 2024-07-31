from locust import HttpUser, between, task

class QuickstartUser(HttpUser):
    wait_time = between(1, 2)
    host = "http://localhost:3411"

    @task
    def sequence(self):
        self.register()
        self.loginhtml()
        self.login()
        self.index()
        #self.chat()
        #self.chat2()
        #self.chat3()
        #self.chat4()
    
    def register(self):
        self.client.post("/register", json={"username": "test", "password": "test"})

    
    def loginhtml(self):
        self.client.get("/login.html")

    
    def login(self):
        self.client.post("/login", json={"username": "test", "password": "test"})
    
    
    def index(self):
        self.client.get("/index.html")

    
    def chat(self):
        self.client.get("/chat/send?q=你好")
    
    
    def chat2(self):
        self.client.get("/chat/send?q=查询")

    
    def chat3(self):
        self.client.get("/chat/send?q=返回")

    
    def chat4(self):
        self.client.get("/chat/send?q=退出")


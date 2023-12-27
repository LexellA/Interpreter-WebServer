from locust import HttpUser, between, task

class QuickstartUser(HttpUser):
    wait_time = between(1, 2)
    host = "http://localhost:3411"
    @task
    def register(self):
        self.client.post("/register", json={"username": "test", "password": "test"})

    @task
    def loginhtml(self):
        self.client.get("/login.html")

    @task
    def login(self):
        self.client.post("/login", json={"username": "test", "password": "test"})
    
    @task
    def index(self):
        self.client.get("/index.html")


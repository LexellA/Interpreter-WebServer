from locust import HttpUser, between, task

class QuickstartUser(HttpUser):
    wait_time = between(1, 2)
    host = "http://localhost:3411"
    # @task
    # def login(self):
    #     self.client.post("/login", json={"username": "test", "password": "test"})

    # @task
    # def register(self):
    #     self.client.post("/register", json={"username": "test", "password": "test"})

    # @task
    # def send_chat(self):
    #     self.client.get("/chat/send", params={"q":"test"})

    @task
    def welcome(self):
        self.client.get("/index.html")

    @task
    def chat(self):
        self.client.get("/chat.html")
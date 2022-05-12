from attacus import App, StandardMethodChannel, StandardMethod

class MyApp(App):
    def __init__(self) -> None:
        super().__init__()
        self.channel = None
        self.counter = 0

    def startup(self):
        print("Starting up ...")
        messenger = self.messenger
        print(messenger)

        channel = self.channel = StandardMethodChannel(messenger, "example")

        StandardMethod(
            channel,
            "incrementCounter",
            lambda method_call, result: self.increment_counter(method_call, result)
        )

        StandardMethod(
            channel,
            "setValue",
            lambda method_call, result: self.set_value(method_call, result)
        )

        print(self.channel)


    def shutdown(self):
        print("Shutting down ...")

    def increment_counter(self, method_call, result):
        print('increment_counter')
        print(method_call)
        print(method_call.method_name())
        print(method_call.arguments())
        print(result)
        self.counter += 1
        result.success(self.counter)

    def set_value(self, method_call, result):
        print('set_counter')
        print(method_call)
        print(method_call.method_name())
        print(method_call.arguments())
        print(result)
        result.success()

app = MyApp()
app.run()

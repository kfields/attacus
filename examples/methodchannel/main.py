from attacus import App, FlutterView, StandardMethodChannel, StandardMethod

class MyFlutter(FlutterView):
    def __init__(self, parent):
        super().__init__(parent)
        self.channel = None
        self.counter = 0

    def startup(self):
        print("Starting up Flutter ...")
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
        print("Shutting down Flutter ...")

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

class MyApp(App):
    def __init__(self):
        super().__init__()

    def startup(self):
        print("Starting up App ...")

    def shutdown(self):
        print("Shutting down App ...")

app = MyApp()
#app = App()

flutter = MyFlutter(app)

app.run()

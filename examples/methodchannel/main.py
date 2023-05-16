from loguru import logger

from attacus import App, FlutterView, StandardMethodChannel, StandardMethod

class MyFlutter(FlutterView):
    def __init__(self, parent):
        super().__init__(parent)
        self.channel = None
        self.counter = 0

    def startup(self):
        logger.debug("Starting up Flutter ...")
        messenger = self.messenger
        logger.debug(messenger)

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

        logger.debug(self.channel)


    def shutdown(self):
        logger.debug("Shutting down Flutter ...")

    def increment_counter(self, method_call, result):
        logger.debug('increment_counter')
        logger.debug(method_call)
        logger.debug(method_call.method_name())
        logger.debug(method_call.arguments())
        logger.debug(result)
        self.counter += 1
        result.success(self.counter)

    def set_value(self, method_call, result):
        logger.debug('set_counter')
        logger.debug(method_call)
        logger.debug(method_call.method_name())
        logger.debug(method_call.arguments())
        logger.debug(result)
        result.success()

class MyApp(App):
    def __init__(self):
        super().__init__()

    def startup(self):
        logger.debug("Starting up App ...")

    def shutdown(self):
        logger.debug("Shutting down App ...")

app = MyApp()

flutter = MyFlutter(app)

app.run()

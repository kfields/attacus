from loguru import logger

from attacus import App, FlutterView, StandardMethodChannel, StandardMethod

class MyChannel(StandardMethodChannel):
    routes = {}

    def __init__(self, messenger, name, receiver):
        super().__init__(messenger, name)
        self.receiver = receiver
        for key, value in self.routes.items():
            StandardMethod(
                self,
                key,
                lambda method_call, result: self.execute(method_call, value, result)
            )

    @classmethod
    def route(cls, method_name):
        def decorator(f):
            cls.routes[method_name] = f
            return f
        return decorator

    def execute(self, method_call, method, result):
        logger.debug(method_call)
        logger.debug(method)
        logger.debug(method_call.method_name())
        args = method_call.arguments()
        logger.debug(args)
        logger.debug(result)

        pyarg = args.decode()
        logger.debug(pyarg)
        method(self.receiver, pyarg)
        result.success()

class MyFlutter(FlutterView):
    def __init__(self, parent):
        super().__init__(parent)
        self.channel = None

    def startup(self):
        logger.debug("Starting up Flutter ...")
        messenger = self.messenger
        logger.debug(messenger)

        self.channel = MyChannel(messenger, "aichat", self)
        logger.debug(self.channel)


    def shutdown(self):
        logger.debug("Shutting down Flutter ...")

    @MyChannel.route('send')
    def send(self, text):
        logger.debug(text)

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

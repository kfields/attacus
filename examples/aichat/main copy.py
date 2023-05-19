from loguru import logger

from attacus import App, FlutterView, StandardMethodChannel, StandardMethod

class MethodRouter:
    def __init__(self):
        self.routes = {}

    def route(self, route_str):
        def decorator(f):
            self.routes[route_str] = f
            return f
        return decorator

    def execute(self, route_str):
        if route_str in self.routes:
            return self.routes[route_str]()
        else:
            raise ValueError(f'Route {route_str} has not been registered')

# create router instance
router = MethodRouter()


class MyFlutter(FlutterView):
    def __init__(self, parent):
        super().__init__(parent)
        self.channel = None

    def startup(self):
        logger.debug("Starting up Flutter ...")
        messenger = self.messenger
        logger.debug(messenger)

        channel = self.channel = StandardMethodChannel(messenger, "aichat")

        StandardMethod(
            channel,
            "send",
            lambda method_call, result: self.send(method_call, result)
        )

        logger.debug(self.channel)


    def shutdown(self):
        logger.debug("Shutting down Flutter ...")

    def send(self, method_call, result):
        logger.debug('send')
        logger.debug(method_call)
        logger.debug(method_call.method_name())
        args = method_call.arguments()
        logger.debug(args)
        logger.debug(result)

        pyarg = args.decode()
        logger.debug(pyarg)

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

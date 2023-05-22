import inspect
import asyncio

from dotenv import load_dotenv

load_dotenv()  # take environment variables from .env.

from loguru import logger

#import google.generativeai as palm
from langchain.chat_models import ChatOpenAI, ChatGooglePalm
from langchain.chains import ConversationChain
from langchain.memory import ConversationBufferMemory

from attacus import App, FlutterView, StandardMethodChannel, StandardMethod

#chat: palm.types.ChatResponse = None

llm = ChatGooglePalm()
#llm = ChatOpenAI()

memory = ConversationBufferMemory()
chain = ConversationChain(llm=llm, memory=memory, verbose=True)

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

    """
    def execute(self, method_call, method, method_result):
        logger.debug(method_call)
        logger.debug(method)
        logger.debug(method_call.method_name())
        args = method_call.arguments()
        logger.debug(args)
        logger.debug(method_result)

        pyarg = args.decode()
        logger.debug(pyarg)
        result = method(self.receiver, pyarg)
        method_result.success(result)
    """
    def execute(self, method_call, method, method_result):
        logger.debug(method_call)
        logger.debug(method)
        logger.debug(method_call.method_name())
        args = method_call.arguments()
        logger.debug(args)
        logger.debug(method_result)

        pyarg = args.decode()
        logger.debug(pyarg)
        #result = None
        if inspect.iscoroutinefunction(method):
            loop = asyncio.get_event_loop()
            def callback(future):
                result = future.result()
                logger.debug(f"Callback invoked with result: {result}")
                method_result.success(result)
            task = loop.create_task(method(self.receiver, pyarg))
            # Add the callback
            task.add_done_callback(callback)
        else:
            result = method(self.receiver, pyarg)
            method_result.success(result)

class MyFlutter(FlutterView):
    def __init__(self, parent):
        super().__init__(parent)
        self.channel = None

    def startup(self):
        super().startup()
        logger.debug("Starting up Flutter ...")
        messenger = self.messenger
        logger.debug(messenger)

        self.channel = MyChannel(messenger, "aichat", self)
        logger.debug(self.channel)


    def shutdown(self):
        super().shutdown()
        logger.debug("Shutting down Flutter ...")

    @MyChannel.route('send')
    async def send(self, text: str):
        global chat
        logger.debug(text)
        """
        if not chat:
            chat = palm.chat(messages=[text])
            response = chat.last
        else:
            response = chat.reply(text)
        """
        #response = await chain.arun(text)
        try:
            response = await chain.arun(text)
            response = response.replace('\n', '\r\n')
        except Exception as e:
            print(f"An error occurred: {e}")

        self.channel.invoke_method('on_message', response)

class MyApp(App):
    def __init__(self):
        super().__init__()

    def startup(self):
        super().startup()
        logger.debug("Starting up App ...")

    """
    def loop(self):
        logger.debug("Entering App Loop ...")
        while self.process_events():
            pass
        logger.debug("Exiting App Loop ...")
    """
    def loop(self):
        logger.debug("Entering App Loop ...")
        def exception_handler(loop, context):
            print(f"Caught exception: {context['exception']}")
        async def _loop(interval=1/60):
            loop = asyncio.get_event_loop()
            loop.set_exception_handler(exception_handler)
            while self.process_events():
                await asyncio.sleep(interval)

        asyncio.run(_loop())
        logger.debug("Exiting App Loop ...")

    def shutdown(self):
        super().shutdown()
        logger.debug("Shutting down App ...")

app = MyApp()

flutter = MyFlutter(app)

app.run()

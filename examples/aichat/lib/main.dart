import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Ai Chat Demo',
      /*theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.blue),
        useMaterial3: true,
      ),*/
      debugShowCheckedModeBanner: false,
      theme: ThemeData.dark(),
      home: const MyHomePage(title: 'Flutter Demo Home Page'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key, required this.title});

  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  String _message = '';
  MethodChannel platform = const MethodChannel('aichat');

  @override
  void initState() {
    super.initState();

    // Register a listener on the channel.
    platform.setMethodCallHandler((MethodCall call) {
      switch (call.method) {
        case 'on_message':
          setState(() {
            _message = call.arguments;
          });
          break;
      }
      return Future(() => null);
    });
  }

  void send(String text) async {
    await platform.invokeMethod('send', text);
    setState(() {
      //userInput = text;
    });
    //print('User input: $userInput');
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        //backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        // Here we take the value from the MyHomePage object that was created by
        // the App.build method, and use it to set our appbar title.
        title: Text(widget.title),
      ),
      body: Center(
        // Center is a layout widget. It takes a single child and positions it
        // in the middle of the parent.
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            Expanded(
              child: Center(
                //child: Text('Main Content Here'),
                child: Text(_message),
              ),
            ),
            Container(
              padding: const EdgeInsets.all(8.0),
              //color: Colors.blue[200],
              child: TextField(
                decoration: const InputDecoration(
                  border: OutlineInputBorder(),
                  labelText: 'message',
                ),
                onSubmitted: send,
              ),
            ),
          ],
        ),
      ),
    );
  }
}

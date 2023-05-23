import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

import 'package:flutter_markdown/flutter_markdown.dart';
import 'package:markdown/markdown.dart' as md;
import 'code_element_builder.dart';
import 'dot_loading_indicator.dart';

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
      debugShowCheckedModeBanner: false,
      theme: ThemeData.dark(useMaterial3: true),
      home: const MyHomePage(title: 'Ai Chat Demo'),
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
  final TextEditingController _messageController = TextEditingController();
  final List<ChatMessage> _messages = [];
  MethodChannel platform = const MethodChannel('aichat');

  @override
  void initState() {
    super.initState();

    // Register a listener on the channel.
    platform.setMethodCallHandler((MethodCall call) {
      switch (call.method) {
        case 'on_message':
          setState(() {
            var text = call.arguments;
            _messages.last.text = text;
            _messages.last.isWaiting = false;
          });
          _scrollController.animateTo(
            _scrollController.position.maxScrollExtent,
            duration: const Duration(milliseconds: 500),
            curve: Curves.easeOut,
          );
          break;
      }
      return Future(() => null);
    });
  }

  void send(String text) async {
    setState(() {
      _messages.add(ChatMessage(
        text: _messageController.text,
        avatar: const Icon(Icons.person, color: Colors.white),
        isWaiting: false,
      ));
      _messages.add(
        ChatMessage(
          text: '',
          avatar: const Icon(Icons.smart_toy_sharp, color: Colors.white),
        ),
      );

      _messageController.clear();
    });
    await platform.invokeMethod('send', text);
  }

  final ScrollController _scrollController = ScrollController();

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
                child: Scrollbar(
                  thumbVisibility: true,
                  controller: _scrollController,
                  child: ListView.builder(
                    controller: _scrollController,
                    itemCount: _messages.length,
                    itemBuilder: (context, index) {
                      return Padding(
                        padding: const EdgeInsets.all(8.0),
                        child: Card(
                          child: ListTile(
                              leading: CircleAvatar(
                                child: _messages[index].avatar,
                              ),
                              title: _messages[index].isWaiting
                                  ? const Center(child: DotLoadingIndicator())
                                  : MarkdownBody(
                                      key:
                                          const Key("defaultmarkdownformatter"),
                                      data: _messages[index].text,
                                      selectable: true,
                                      extensionSet: md.ExtensionSet(
                                        md.ExtensionSet.gitHubFlavored
                                            .blockSyntaxes,
                                        [
                                          md.EmojiSyntax(),
                                          ...md.ExtensionSet.gitHubFlavored
                                              .inlineSyntaxes
                                        ],
                                      ),
                                      builders: {
                                          'code': CodeElementBuilder(
                                              context: context),
                                        })),
                        ),
                      );
                    },
                  ),
                ),
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
                controller: _messageController,
                onSubmitted: send,
              ),
            ),
          ],
        ),
      ),
    );
  }
}

class ChatMessage {
  String text;
  //final ImageProvider avatar;
  final Icon avatar;
  bool isWaiting;
  ChatMessage(
      {required this.text, required this.avatar, this.isWaiting = true});
}

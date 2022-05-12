import 'package:rxdart/rxdart.dart';
import 'package:flutter/services.dart';
import 'package:flutter/material.dart';

class ToggleControl {
  final _subject = BehaviorSubject<bool>();
  String name = '';
  MethodChannel platform;
  ToggleControl(this.name) : platform = MethodChannel(name);

  /// Add data to the stream
  void setValue(bool value) async {
    await platform.invokeMethod('setValue', value);
    _subject.sink.add(value);
  }

  Widget build(Widget Function(BuildContext, AsyncSnapshot<bool>) fn) {
    return StreamBuilder(stream: observable, builder: fn);
  }

  /// Validate and retrieve data from the stream
  Stream<bool> get observable => _subject.stream;
}

class ButtonControl {
  final _subject = BehaviorSubject<bool>();
  String name = '';
  MethodChannel platform;
  ButtonControl(this.name) : platform = MethodChannel(name);

  /// Add data to the stream
  void setValue(bool value) async {
    await platform.invokeMethod('setValue', value);
    _subject.sink.add(value);
  }

  Widget build(Widget Function(BuildContext, AsyncSnapshot<bool>) fn) {
    return StreamBuilder(stream: observable, builder: fn);
  }

  /// Validate and retrieve data from the stream
  Stream<bool> get observable => _subject.stream;
}

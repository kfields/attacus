import 'package:flutter/material.dart';
import 'dart:math';

class DotLoadingIndicator extends StatefulWidget {
  const DotLoadingIndicator({super.key});
  @override
  DotLoadingIndicatorState createState() => DotLoadingIndicatorState();
}

class DotLoadingIndicatorState extends State<DotLoadingIndicator>
    with SingleTickerProviderStateMixin {
  late AnimationController _controller;
  late Animation<double> _animation;

  @override
  void initState() {
    super.initState();
    _controller = AnimationController(
      duration: const Duration(milliseconds: 1000),
      vsync: this,
    )..repeat(reverse: true);

    _animation = CurvedAnimation(
      parent: _controller,
      curve: Curves.easeInOut,
    );
  }

  @override
  Widget build(BuildContext context) {
    return Row(
      mainAxisAlignment: MainAxisAlignment.center,
      children: List.generate(
        3,
        (index) => Padding(
          padding: const EdgeInsets.symmetric(horizontal: 2.0),
          child: ScaleTransition(
            scale: DelayTween(begin: 0.0, end: 1.0, delay: 0.2 * index)
                .animate(_animation),
            child: Container(
              width: 10.0,
              height: 10.0,
              decoration: const BoxDecoration(
                shape: BoxShape.circle,
                color: Colors.blue,
              ),
            ),
          ),
        ),
      ),
    );
  }

  @override
  void dispose() {
    _controller.dispose();
    super.dispose();
  }
}

class DelayTween extends Tween<double> {
  DelayTween({double begin = 0.0, double end = 1.0, this.delay = 0.0})
      : super(begin: begin, end: end);

  final double delay;

  @override
  double lerp(double t) => super.lerp((sin((t - delay) * 2 * pi) + 1) / 2);
}

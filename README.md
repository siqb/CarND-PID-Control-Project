# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---
## Effects of P, I, and D components

In this project, I implemented a PID controller for steering. 

P represents the amount of steering correction that is done in proportion to the amount of error from the refernce trajectory. In other words, the farther the vehicle is from the desired path, the harder the PID controller will pull the steering wheel in the other direction. This works very well to get the car back on track when it deviates from the path on straight parts of the track.

D represents the amount of steering correction that is done in proportion to the rate of change that the vehcile deviated from its reference trajectory. In other words, when the car *suddenly* finds itself off its desired path (like at a curve in the road), the PID controller has the pull the sterring wheel in the correct direction even harder than in the P step above! 

I represents the amount of steering correction that is done in proporition to the overall bias that the vehicle maintains over time. Bias can come from imperfectly tuned P and D components. It can also come from things like wind and other constant forces that our motion model does not account for.

## How final hyperparameters were chosen

I used a combination of manual tuning and the twiddle algorithm to tune the final parameters. Through experimentation, I chose initial Kp, Ki, and Kd values of 

  #define KP_INIT 0.08
  #define KI_INIT 0.0009
  #define KD_INIT 3.8 

P was tuned by seeing what value made the car stay towards the center on the intial straight potion of the track. If the car was swinging too much, it meant that it was overcorrecting for small errors and that Kp was too big. If it was too small, the car would undercorrect for error and end up off road even in the straight segments. 

I found that the most important parameter was KD. It was very hard to get around the sharp curves without being very aggressive with this coefficient. I was surprised how much I had to increase it to be able to react to the curves fast enough!

The hardest parameter to understand the impact of is KI. The reason is that bias is a long term integral and as such, its hard to understand what kind of impact it's having on the car's path by tweaking it by small amounts. Therefore, I focused mainly on optimizing P and D.

Then, the twiddle algorithm converged on final values from here. I'm not sure if I implemented twiddle correctly though. I did find that it converged on final values very fast. As such, it was very important to select correct initial values. Doesn't that defeat the purpose of twiddle? In any case, I was able to see how twiddle adjusts the values of the coefficients based on if they are having a positive or negative impact on the cross track error.


## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

There's an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Project Instructions and Rubric

Note: regardless of the changes you make, your project must be buildable using
cmake and make!

More information is only accessible by people who are already enrolled in Term 2
of CarND. If you are enrolled, see [the project page](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/f1820894-8322-4bb3-81aa-b26b3c6dcbaf/lessons/e8235395-22dd-4b87-88e0-d108c5e5bbf4/concepts/6a4d8d42-6a04-4aa6-b284-1697c0fd6562)
for instructions and the project rubric.

## Hints!

* You don't have to follow this directory structure, but if you do, your work
  will span all of the .cpp files here. Keep an eye out for TODOs.

## Call for IDE Profiles Pull Requests

Help your fellow students!

We decided to create Makefiles with cmake to keep this project as platform
agnostic as possible. Similarly, we omitted IDE profiles in order to we ensure
that students don't feel pressured to use one IDE or another.

However! I'd love to help people get up and running with their IDEs of choice.
If you've created a profile for an IDE that you think other students would
appreciate, we'd love to have you add the requisite profile files and
instructions to ide_profiles/. For example if you wanted to add a VS Code
profile, you'd add:

* /ide_profiles/vscode/.vscode
* /ide_profiles/vscode/README.md

The README should explain what the profile does, how to take advantage of it,
and how to install it.

Frankly, I've never been involved in a project with multiple IDE profiles
before. I believe the best way to handle this would be to keep them out of the
repo root to avoid clutter. My expectation is that most profiles will include
instructions to copy files to a new location to get picked up by the IDE, but
that's just a guess.

One last note here: regardless of the IDE used, every submitted project must
still be compilable with cmake and make./

## How to write a README
A well written README file can enhance your project and portfolio.  Develop your abilities to create professional README files by completing [this free course](https://www.udacity.com/course/writing-readmes--ud777).


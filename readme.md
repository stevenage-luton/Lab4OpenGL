# COMP3015 CW1 Huw Jenkins
This is the codebase for my Coursework 1 Street Scene based on early PS1 games such as Resident Evil and Silent Hill.
## Which version Visual Studio and Operating System you used to test/write the code locally?
Visual Studio 2022, Microsoft Windows 11.
## How does it work? How does your code fit together and how should a programmer navigate it?
We store the positions for every object in the scene, setting them in Init. We then set up the lights and fog. In the render loop, we draw the skybox first without a depth test and with the normalmatrix instead of the view matrix. A gradient is added to the skybox in the fragment shader to blend it with the fog. Then for each object in the scene, we bind the correct texture to the shader 2Dsampler, and render the object. The fragment shader uses BlinnPhong shading, and we have alpha clipping and fog applied to the final FragColour.

For lights, we iterate through every light in the scene except for the red light, and draw it in a fixed position before anything else. The red light is drawn after, and has it's direction changed following a sine wave that's changed in the update function.

Keyboard and Mouse inputs are done using custom functions added to the Scenerunner.h helper file. Virtual methods are in the Scene class, which are implemented in SceneBasic_Uniform and are used to move the camera with a given vector, which is provided by the functions added to Scenerunner.
## Anything else which will help us to understand how your prototype works.
WASD to move, Mouse to look. There's no collision so you can go and see the backfaces of the shops if you'd like.
## A Link to the unlisted YouTube Video 
https://youtu.be/iSQ5LW7fvnM

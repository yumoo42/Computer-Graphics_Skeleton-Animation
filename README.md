# Computer-Graphics_Skeleton-Animation
## 1. Environment
Ubuntu 18.04.6 LTS / Visual Studio Code / C++

## 2. Method Description
The overall structure is modified based on https://ogldev.org/www/tutorial38/tutorial38.html.

### Modifications in model.dae
Added specific code to the model file to apply texture.

### BoneTransform()
Calculates the transformation matrix for skeletal animation.
- Initialize an identity matrix.
- Calculate the animation speed per second. If mTicksPerSecond is 0, use the default value of 25.0f.
- Use ReadNodeHierarchy() to read the node hierarchy structure and calculate the final transformation matrix for the skeleton.
- Adjust the size of Transforms to store the transformation matrix of each bone.
- Save the computed transformation matrices into Transforms.
### ReadNodeHierarchy()
Recursively reads the node hierarchy structure and computes the transformation matrices.
- Retrieve the current node's name, animation data, and original transformation matrix.
- If the node has animation data, compute its transformation matrix by interpolating scaling, rotation, and translation values based on the animation data, then combine these into the transformation matrix.
- If the node corresponds to a bone, update the bone's final transformation matrix.
### UpdateModelPosition()
Calculates the model's position along a circular path.

### GetModelHeading()
Calculates the model's heading (tangential direction of the circular path).

### RenderSceneCB()
Renders the scene.
- Create a vector to store transformation matrices and initialize transformation pipelines for global transformations (camera and perspective projection) and model transformations (scaling, rotation, and translation).
- If the animation is running:
  - Calculate the transformation matrix for skeletal animation.
  - Update the model's position.
  - Rotate the model based on its computed heading.
  - Save the current time, position, and heading.
- If the animation is paused, use the previously stored heading and position.
- Set the model's position in global space.
- Pass the transformation matrices for skeletal animation, camera position, and the global-view-projection matrix to the shader.
- Finally, render the model.
## 3. How to Run the Program
Run **./build.sh** to build the project and generate the hw4 executable file.\
Execute the program by running **./hw4**\
Press the s key to start or pause the animation.\
Use the mouse to adjust the camera's position.\
Press the q key to exit the program.



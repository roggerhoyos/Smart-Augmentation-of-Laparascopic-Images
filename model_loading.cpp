
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
#include "model.h"

#include <iostream>


//All Callbacks funtions  are called without been in the Main Loop
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f)); //set Camera position 
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true; 

// timing
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

int main()
{
    //1ST: INITIALIZE GLFW
	glfwInit(); 



	// //2STEP : create a window
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //Max. 4
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  //Max. 3
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SMART AUGMENTATION OF LAPAROSCOPIC IMAGES", NULL, NULL);
	
	if (window == NULL) //if Window was not created 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	 
	glfwMakeContextCurrent(window); //Set the Version to our window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //Called when window is resized. Ajust the viewport (via PollEvent)
	glfwSetCursorPosCallback(window, mouse_callback); //Called when  move the mouse (via PollEvent)
	glfwSetScrollCallback(window, scroll_callback);  //Called when move the  scroll (via PollEvent)
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // tell GLFW to capture our mouse



	// 3TH: Initialize GLAD: load all OpenGL function pointers (Need WINDOW  and OpenGL context)
	//we pass GLAD the function to load the adress of the OpenGL function pointers 
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}



	//4TH STEP: OpenGL Options could be changed!!!

	//glEnable(GL_DEPTH_TEST); //Enable Depth buffer. Depth test occur after Fragment Shader. Compare fragment with the deth buffer
	
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);

	//glEnable(GL_BLEND); //Blend Fragment color values
 //  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // draw in wireframe



	//5TH STEP: Initialize shaders

	//Sillhouette
//	Shader vesselModelShader("vertex_vesselModel_shader.glsl", "fragment_vesselModel_shader.glsl"); // build and compile shaders
	Shader vesselDepthShader("vertex_depth_shader.glsl", "fragment_depth_shader.glsl");
	Shader silhouetteShader("framebuffers_silhouette_vertex.glsl", "framebuffers_silhouette_fragment.glsl");
	Shader varSilhouetteShader("vertex_varsilhouette_shader.glsl", "fragment_varsilhouette_shader.glsl");
	Shader changeColor("vertex_changeColor_shader.glsl", "fragment_changeColor_shader.glsl");
	

	//Zebra Stripe
	Shader zebraShader("framebuffer_zebra_stripe_vertex.glsl", "framebuffer_zebra_stripe_fragment.glsl");


	//Screen
	Shader screenShader("screen_shader_vertex.glsl", "screen_shader_fragment.glsl");
	Shader vesselTextureShader("vertex_vesselTexture_shader.glsl", "fragment_vesselTexture_shader.glsl");

	//Until Now, the Program (ID) was created (attaching the Shaders)



	//6TH: Quad fit Entire Screen 
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	


	//7TH: BUFFERS FOR THE QUAD
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));



	//8th STEP: Initialize Model
	Model ourModel("PMSD/vessel.obj"); // load models

	glfwSetCursorPos(window, lastX, lastY); //Avoid cursor jump at start
	


	//9TH STEP: Shader configuration
	// --------------------
	//Sillhouette
 //   vesselModelShader.use();
 //   vesselModelShader.setInt("vesselTexture", 0);
	silhouetteShader.use();
	silhouetteShader.setInt("silhouetteTexture", 0);
	vesselDepthShader.use();
	vesselDepthShader.setInt("depthTexture", 0);
	varSilhouetteShader.use();
	varSilhouetteShader.setInt("silhouetteTexture", 0);
	changeColor.use();
	changeColor.setInt("changeColorTexture",0);


	//Zebra Stripe
	zebraShader.use();
	zebraShader.setInt("vesselTexture", 0);
	

	//Screen
	screenShader.use();
	screenShader.setInt("silhouetteTexture", 0);
	screenShader.setInt("zebraTexture", 1);
//	screenShader.setInt("silhouetteVarTexture",2);
	
	vesselTextureShader.use();
	vesselTextureShader.setInt("FinalTexture", 0);




	//10TH STEP: framebuffer configuration
	enum tasks {VESSELMODEL=0, SILHOUETTE, ZEBRASTRIPE, DEPTH, VARSILHOUETTE, FINAL, CHANGECOLOR};
	unsigned int framebuffer[7];
	glGenFramebuffers(7, framebuffer);
	// create a color attachment texture
	unsigned int textureColorbuffer[7];
	glGenTextures(7, textureColorbuffer);
	for (int i = 0; i < 7; ++i) {
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[i]);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer[i], 0);
	}
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	 //for (int i = 0; i < 3; ++i) {
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	//}
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);




	/*1. Time per frame
	  2. Update inputs: keyboard, Camera, Mouse, Scroll
	  3. Clear the Screen
	  4. Rendering
	  5. Free up Memory*/


	//11TH: Main Loop
	while (!glfwWindowShouldClose(window)) //ESC KEY  to Quit (If True quit the Loop)
	{
		// a.per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// b. input: Keyboard, Camera, Mouse
		processInput(window);

     
        //SILLHOUETTE TEXTURE TASKS 
		// c. Enable Frame Buffer Object (Off-Screen )
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[DEPTH]);
		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
		
		// d. Clear the framebuffer's content
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //Black Window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear buffer

		// e. Enable Program that contain the Shaders.
		vesselDepthShader.use();

		// f. view/projection transformations
		glm::mat4 projection_1 = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); //prohection matrix type Projective. Zoom = fov, 1 and 100, near and far plane respectively
		glm::mat4 view_1 = camera.GetViewMatrix(); //Camera or Eye view. Return the LookAt Matrix
		vesselDepthShader.setMat4("projection", projection_1);
		vesselDepthShader.setMat4("view", view_1);

		// g. draw (Off-Screen rendering)
		glm::mat4 model_1 = glm::mat4(1.0f);
		model_1 = glm::translate(model_1, glm::vec3(0.0f, -1.75f, -39.0f)); // translate it down so it's at the center of the scene
		model_1 = glm::rotate(model_1, glm::radians(45.f), glm::vec3(1.0, 1.0, 1.0));
		model_1 = glm::scale(model_1, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		vesselDepthShader.setMat4("model", model_1); //transform all object's vertices it to the global world space
		ourModel.Draw(vesselDepthShader); //send the active shader

		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[SILHOUETTE]); 
		glEnable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test. //DISABLE
		// clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);

		silhouetteShader.use();
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer[DEPTH]);	// use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);


		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[VARSILHOUETTE]);
		glEnable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test. //DISABLE
		// clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);

		varSilhouetteShader.use();
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer[SILHOUETTE]);	// use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);


	    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[CHANGECOLOR]);
        glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
       // clear all relevant buffers
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT);

		changeColor.use();
        glBindVertexArray(quadVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer[VARSILHOUETTE]);
        glDrawArrays(GL_TRIANGLES, 0, 6);

		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	 //   glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	 //   // clear all relevant buffers
	 //   glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	 //   glClear(GL_COLOR_BUFFER_BIT);

		//changeColor.use();
	 //   glBindVertexArray(quadVAO);
	 //   glActiveTexture(GL_TEXTURE0);
	 //   glBindTexture(GL_TEXTURE_2D, textureColorbuffer[CHANGECOLOR]);
	 //   glDrawArrays(GL_TRIANGLES, 0, 6);



	   //ZEBRA STRIPE TEXTURE  TASKS
	   // c. Enable Frame Buffer Object (Off-Screen )
	   glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[ZEBRASTRIPE]);
	   glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

	   //// d. Clear the framebuffer's conten
	   glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //Black Window
	   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear buffer

	   //// e. Enable Program that contain the Shaders.
	   zebraShader.use();

	   // f. view/projection transformations
	   glm::mat4 projection_2 = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); //prohection matrix type Projective. Zoom = fov, 1 and 100, near and far plane respectively
	   glm::mat4 view_2 = camera.GetViewMatrix(); //Camera or Eye view. Return the LookAt Matrix
	   zebraShader.setMat4("projection", projection_2);
	   zebraShader.setMat4("view", view_2);

	   // g. draw ((Off-Screen rendering))
	   glm::mat4 model_2 = glm::mat4(1.0f);
	   model_2 = glm::translate(model_2, glm::vec3(0.0f, -1.75f, -39.0f)); // translate it down so it's at the center of the scene
	   model_2 = glm::rotate(model_2, glm::radians(45.f), glm::vec3(1.0, 1.0, 1.0));
	   model_2 = glm::scale(model_2, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
	   zebraShader.setMat4("model", model_2); //transform all object's vertices it to the global world space
	   ourModel.Draw(zebraShader); //send the active shader
	  
			  
	   //SCREEN
	   //h. draw (Screen-Rendering) 
	   // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	   glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[FINAL]);
	   glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	   // clear all relevant buffers
	   glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	   glClear(GL_COLOR_BUFFER_BIT);
			  
	   screenShader.use();
	   glBindVertexArray(quadVAO);
	   glActiveTexture(GL_TEXTURE0);
	   glBindTexture(GL_TEXTURE_2D, textureColorbuffer[CHANGECOLOR]);	// use the color attachment texture as the texture of the quad plane
	   glActiveTexture(GL_TEXTURE1);
	   glBindTexture(GL_TEXTURE_2D, textureColorbuffer[ZEBRASTRIPE]);
	   glDrawArrays(GL_TRIANGLES, 0, 6);
	   

	   glBindFramebuffer(GL_FRAMEBUFFER, 0);
	   glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	   // clear all relevant buffers
	   glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	   glClear(GL_COLOR_BUFFER_BIT);

	   vesselTextureShader.use();
	   glBindVertexArray(quadVAO);
	   glActiveTexture(GL_TEXTURE0);
	   glBindTexture(GL_TEXTURE_2D, textureColorbuffer[FINAL]);
	   glDrawArrays(GL_TRIANGLES, 0, 6);


		// i. end draw
		glfwSwapBuffers(window); //Swap from back buffer to front buffer
		glFlush(); //ensures that previous OpenGL commands complete in finite time
		glfwPollEvents();  // let the cursor interact with the windowv
	}


	//12TH: End of program 
	glfwTerminate(); //clean all. Free up your memory
	return 0;
}




// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);        //ESC  (Set to true )

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);    //W KEY
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
		camera.ProcessKeyboard(BACKWARD, deltaTime);  //S KEY
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);      //A KEY
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);    //D KEY

	//if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	// 	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height); //Create Viewport same size of our framebuffer
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos) //xpos and ypos represent the current mouse positions
{
	//1.Calculate the mouse's offset since the last frame.

	if (firstMouse)// this bool variable is initially set to true	
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}



























////SILLHOUETTE TEXTURE TASKS 
//// c. Enable Frame Buffer Object (Off-Screen )
//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[VESSELMODEL]);
//glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
//
//// d. Clear the framebuffer's content
//glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //Black Window
//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear buffer

//// e. Enable Program that contain the Shaders.
//vesselModelShader.use();

//// f. view/projection transformations
//glm::mat4 projection_1 = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); //prohection matrix type Projective. Zoom = fov, 1 and 100, near and far plane respectively
//glm::mat4 view_1 = camera.GetViewMatrix(); //Camera or Eye view. Return the LookAt Matrix
//vesselModelShader.setMat4("projection", projection_1);
//vesselModelShader.setMat4("view", view_1);

//// g. draw (Off-Screen rendering)
//glm::mat4 model_1 = glm::mat4(1.0f);
//model_1 = glm::translate(model_1, glm::vec3(0.0f, -1.75f, -39.0f)); // translate it down so it's at the center of the scene
//model_1 = glm::rotate(model_1, glm::radians(45.f), glm::vec3(1.0, 1.0, 1.0));
//model_1 = glm::scale(model_1, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
//vesselModelShader.setMat4("model", model_1); //transform all object's vertices it to the global world space
//ourModel.Draw(vesselModelShader); //send the active shader

//// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[SILHOUETTE]); 
//glEnable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test. //DISABLE
//// clear all relevant buffers
//glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
//glClear(GL_COLOR_BUFFER_BIT);

//silhouetteShader.use();
//glBindVertexArray(quadVAO);
//glBindTexture(GL_TEXTURE_2D, textureColorbuffer[VESSELMODEL]);	// use the color attachment texture as the texture of the quad plane
//glDrawArrays(GL_TRIANGLES, 0, 6);


//      //ZEBRA STRIPE TEXTURE  TASKS
	  //// c. Enable Frame Buffer Object (Off-Screen )
	  //glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[ZEBRASTRIPE]);
	  //glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

	  ////// d. Clear the framebuffer's conten
	  //glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //Black Window
	  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear buffer

	  ////// e. Enable Program that contain the Shaders.
	  //zebraShader.use();

	  //// f. view/projection transformations
   //   glm::mat4 projection_2 = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); //prohection matrix type Projective. Zoom = fov, 1 and 100, near and far plane respectively
   //   glm::mat4 view_2 = camera.GetViewMatrix(); //Camera or Eye view. Return the LookAt Matrix
	  //zebraShader.setMat4("projection", projection_2);
	  //zebraShader.setMat4("view", view_2);

	  //// g. draw ((Off-Screen rendering))
	  //glm::mat4 model_2 = glm::mat4(1.0f);
	  //model_2 = glm::translate(model_2, glm::vec3(0.0f, -1.75f, -39.0f)); // translate it down so it's at the center of the scene
	  //model_2 = glm::rotate(model_2, glm::radians(45.f), glm::vec3(1.0, 1.0, 1.0));
	  //model_2 = glm::scale(model_2, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
	  //zebraShader.setMat4("model", model_2); //transform all object's vertices it to the global world space
	  //ourModel.Draw(zebraShader); //send the active shader

	  //
	  //
	  ////SCREEN
	  ////h. draw (Screen-Rendering) 
	  //// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	  //glBindFramebuffer(GL_FRAMEBUFFER, 0);
	  //glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	  //// clear all relevant buffers
	  //glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	  //glClear(GL_COLOR_BUFFER_BIT);
	  //
	  //screenShader.use();
	  //glBindVertexArray(quadVAO);
	  //glActiveTexture(GL_TEXTURE0);
	  //glBindTexture(GL_TEXTURE_2D, textureColorbuffer[SILHOUETTE]);	// use the color attachment texture as the texture of the quad plane
	  //glActiveTexture(GL_TEXTURE1);
	  //glBindTexture(GL_TEXTURE_2D, textureColorbuffer[ZEBRASTRIPE]);
	  //glActiveTexture(GL_TEXTURE2);
	  //glBindTexture(GL_TEXTURE_2D, textureColorbuffer[DEPTH]);
	  //glDrawArrays(GL_TRIANGLES, 0, 6);

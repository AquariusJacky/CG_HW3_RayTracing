# 2020 CG HW3 Spec
###### tags: `NCTU Computer Graphics`

* [HW3 Hint](https://docs.google.com/presentation/d/1T9rWakOsig8m2jIDbE45_k8kjhQbIh3a-vYKckpt6GQ/edit?usp=sharing)
* HW3 DueDate: 12/07 23:59:59
* No need to Demo, so please briefly explain the code in the Report. However, we will still execute your code, if there's any problem, we'll send email to you.
---

:::info 

***Computer Graphics 2020 - Homework 3***

In this homework, you’re required to write a program that implements Ray Tracing Effect.

:::
* In CG_HW3_EXAMPLE, you can display a colorful image and know how to save the image to .ppm file.

* Follow the TODO instructions and write your code in project CG_HW3.

* You need to implement 3 files including **main.cpp**, **ray.h**, and **geo.h** in CG_HW3.

## Object Overview

* Overview:

    1. Render a ball with diffuse material. (20%)

    2. Render a ball with refracted/transmitted ray. (20%)

    3. Render a ball with total internal reflected ray. (10%)

    4. Render the shadows of all the balls on the ground. (20%)

    5. Save a result image to ppm format and convert it to bmp/jpg/png format. (10%)
    
    6. Report (CH/EN is available) (pdf) (20%)
    
    7. Bonus: Any other design can be a plus. (10%)

* Preview (Click the Picture for full resolution pictures)

    | Result |
    | -------- |
    |[![](https://imgur.com/DiBhOCm.jpg =600x)](https://imgur.com/DiBhOCm.jpg) |



## Details in each part

1. Render a ball with diffuse material. (20%)

    :::info
    Diffuse equation:
    
    ![](https://i.imgur.com/ygbqbkA.png =400x)
    :::
    
    ```
    L = LightPosition = (-10.0, 10.0, 0.0)
    I = LightIntensity = (1.0, 1.0, 1.0)
    Kd has already defined in the release code.
    ```
    * Compute the render color with diffuse lighting.

        [![](https://i.imgur.com/ixpe9cd.png =200x)](https://i.imgur.com/ixpe9cd.png)

2. Render a ball with refracted/transmitted ray. (20%)
    :::info
    Refracted direction equation (from course ppt):
    
    ![](https://i.imgur.com/qdX0lEP.png =600x)
    :::
    ```
    transmitted_ray = ray(intersect_pos, refract(射線方向d, 法向量n, 折射率nt));
	transmitted = trace(transmitted_ray, list, depth + 1);
    ```
    * Compute the refracted(transmitted) direction and use recursive ray tracing to render the refracted color.
    *  Refractive indices
        *  air: 1
        *  ball: 1.5

        [![](https://i.imgur.com/sBX9ikf.png =200x)](https://i.imgur.com/sBX9ikf.png)

3. Render a ball with total internal reflected ray. (10%)
    :::info
    Reflected direction equation (from course ppt):
    
    ![](https://i.imgur.com/h9QnLSa.png =600x)
    :::

    ```
    reflected_ray = ray(intersect_pos, reflect(射線方向d, 法向量n));
	reflected = trace(reflected_ray, list, depth + 1);
    ```
    * Compute the reflected direction and use recursive ray tracing to render the reflected color.
    
        [![](https://imgur.com/5Ash6ZI.png =200x)](https://imgur.com/5Ash6ZI.png)

4. Render the ground shadows of all the balls. (20%)
    :::info
    Shadow Ray direction:
    
    ![](https://i.imgur.com/wwuZQwN.png =300x)
    :::

    * Be careful to the shadow rounding error. (Should not have noise in the image.)

        [![](https://i.imgur.com/6RR7War.png =200x)](https://i.imgur.com/6RR7War.png)


5. Save a result image to ppm format and convert it to bmp/jpg/png format. (10%)
    * Resolution of result image: ( **width = 1280, height = 720** )
    
    * The result image should **include 3 big balls** and some **random small balls** with the requirements above. 
    
    * You can search for the file converting methods from the internet.
    
    * You must **convert the image by code** instead of converting it online.
6. Report (CH/EN is available) (pdf) (20%)
    
    * Save your report as a **.pdf** file
        * font size: 12
    
    * Put the **screenshot of your code** and briefly explain it.
    
    * Explain how you implement the above reqirements **in detail**, since there is no demo for this homework.

    * Put the **result image** in your report.
    
    * Describe the problems you met and how you solved them.
    
    * Illustrate extra features of your design. (optional)
    
7. Bonus: Any other design can be a plus.
    * Be creative! All extra features are welcomed! :+1: 
    
    * Some Recommendations:
        * Different camera view.
        * Attenuation in shadow ray.

## Deadline

Please hand in your zip file using E3, deadline will be at **2020/12/07 23:59:59**.

Penalty of 10 points of the value of the assignment per late day. 


## Running Enviornment

Visual Studio 2019 is recommended.

You may specify your target environment by providing a ```README``` or ```MAKEFILE```, but it’s better to inform the TAs beforehand, just in case we cannot execute your program.

## Upload Format

:::danger
If your uploading format doesn't match our requirement, there will be penalty to your score.
:::

Please hand in your project file, report(.pdf) and 2 result images(.ppm, .bmp/jpg/png) as ```STUDENTID_hw3.zip``` ​ to e3 platform.

e.g. ```309123456_hw3.zip```


## References
* [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
* [Refraction](https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel)
* [PPM Viewer](http://www.cs.rhodes.edu/welshc/COMP141_F16/ppmReader.html)

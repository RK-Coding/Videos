#Artifical intelligence faces, followed tutorial
#I also debugged my docker issue, and will fix that over the weekend.

import numpy as np
import tensorflow as tf

# Load TFLite model and allocate tensors.
interpreter = tf.lite.Interpreter(model_path="tutorial1.tflite")
interpreter.allocate_tensors()

def sample():
  input_details = interpreter.get_input_details()
  output_details = interpreter.get_output_details()

  # Set the 'latent' input tensor.
  input_shape = input_details[0]['shape']
  latent = (np.random.random_sample(input_shape) - 0.5) * 2.0
  input_data = np.array(latent, dtype=np.float32)
  interpreter.set_tensor(input_details[0]['index'], input_data)

  interpreter.invoke()

  # Get the output image and transform it for display
  result = interpreter.get_tensor(output_details[0]['index'])
  result = np.reshape(result, [256,256,3])
  result = (result + 1.0) * 127.5
  result = pygame.surfarray.make_surface(result)
  result = pygame.transform.rotate(result, -90)
  return result

import pygame
pygame.init()
display = pygame.display.set_mode((300, 300))

surface = sample()
running = True

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_SPACE:
                surface = sample()

    display.blit(surface, (0, 0))
    pygame.display.update()
pygame.quit()

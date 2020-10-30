from math import sin, cos
import pygame

class Window:
    def __init__(self, width, height, title):
        self.width = width
        self.height = height
        self.title = title

        self.screen = pygame.display.set_mode((self.width, self.height))
        pygame.display.set_caption(self.title)

    def key_check(self, key):
        keys = pygame.key.get_pressed()

        if (keys[key]):
            return True

    def update(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                exit()

        pygame.display.update()

class Mesh:
    def __init__(self, verts, edges, x, y, z, rotX, rotY, rotZ, scale):
        self.verts = verts
        self.edges = edges
        self.x = x
        self.y = y
        self.z = z
        self.rotX = rotX
        self.rotY = rotY
        self.rotZ = rotZ
        self.scale = scale

    def project_and_rotate(self, x, y, z):
        px = (((x * cos(self.rotZ) - sin(self.rotZ) * y) * cos(self.rotY) - z * sin(self.rotY)) * (200 / ((((z * cos(self.rotY) + (x * cos(self.rotZ) - sin(self.rotZ) * y) * sin(self.rotY)) * cos(self.rotX) + (y * cos(self.rotZ) + x * sin(self.rotZ)) * sin(self.rotX)) + 5) + self.z))) * self.scale + self.x
        py = (((y * cos(self.rotZ) + x * sin(self.rotZ)) * cos(self.rotX) - (z * cos(self.rotY) + (x * cos(self.rotZ) - sin(self.rotZ) * y) * sin(self.rotY)) * sin(self.rotX)) * (200 / ((((z * cos(self.rotY) + (x * cos(self.rotZ) - sin(self.rotZ) * y) * sin(self.rotY)) * cos(self.rotX) + (y * cos(self.rotZ) + x * sin(self.rotZ)) * sin(self.rotX)) + 5) + self.z))) * self.scale + self.y
        
        return (int(px), int(py))

    def render(self, window):
        window.screen.fill((0, 0, 0))

        for vert in self.verts:
            point = self.project_and_rotate(vert[0], vert[1], vert[2])

            pygame.draw.circle(window.screen, (0, 255, 0), point, 6)

        for edge in self.edges:
            point1 = self.project_and_rotate(self.verts[edge[0]][0], self.verts[edge[0]][1], self.verts[edge[0]][2])
            point2 = self.project_and_rotate(self.verts[edge[1]][0], self.verts[edge[1]][1], self.verts[edge[1]][2])

            pygame.draw.line(window.screen, (0, 255, 0), point1, point2, 5)

window = Window(500, 500, "Cube")
cube = Mesh([(0, 0, 0), (0, 1, 0), (1, 1, 0), (1, 0, 0), (0, 0, -1), (0, 1, -1), (1, 1, -1), (1, 0, -1)], [(0, 1), (1, 2), (2, 3), (3, 0), (4, 5), (5, 6), (6, 7), (7, 4), (0, 4), (1, 5), (2, 6), (3, 7)], 250, 250, 5, 0, 0, 0, 5)

while True:
    cube.rotX += 0.001
    cube.rotY += 0.01
    cube.rotZ += 0.001

    cube.render(window)
    window.update()
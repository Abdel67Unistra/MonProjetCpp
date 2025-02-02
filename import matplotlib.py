import matplotlib.pyplot as plt
import numpy as np

# Paramètres
num_points = 500  # Nombre de points à générer
r = 1  # Rayon du cercle (et côté du carré)

# Génération des points aléatoires
x_points = np.random.uniform(0, r, num_points)
y_points = np.random.uniform(0, r, num_points)

# Vérification si les points sont dans le quart de cercle
inside_circle = x_points**2 + y_points**2 <= r**2

# Création du graphique
fig, ax = plt.subplots(figsize=(8, 8))
ax.set_aspect('equal')

# Tracer le carré
square = plt.Rectangle((0, 0), r, r, color="lightblue", alpha=0.3, label="Square Area (r²)")
ax.add_patch(square)

# Tracer le quart de cercle
circle = plt.Circle((0, 0), r, color="lightgreen", alpha=0.5, label="Quarter Circle (πr²/4)")
ax.add_patch(circle)

# Points aléatoires
ax.scatter(x_points[inside_circle], y_points[inside_circle], color="green", label="Points inside", s=10)
ax.scatter(x_points[~inside_circle], y_points[~inside_circle], color="red", label="Points outside", s=10)

# Légende et annotations
ax.legend()
plt.title("Monte Carlo Method for Estimating π")
plt.xlabel("x")
plt.ylabel("y")

# Affichage
plt.xlim(0, r)
plt.ylim(0, r)
plt.grid(alpha=0.3)
plt.show()

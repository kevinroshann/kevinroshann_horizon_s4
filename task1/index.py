import matplotlib.pyplot as plt

def distance(p1, p2):
    return ((p1[0] - p2[0]) ** 2 + (p1[1] - p2[1]) ** 2) ** 0.5


def find_nearest_neighbor(points):
    lines = []
    for i in range(len(points)):
        min_dist = float('inf') # start with largest possible number
        nearest_index = -1
        for j in range(len(points)):
            if i == j:
                continue 
            dist = distance(points[i], points[j])
            if dist < min_dist:
               
                if (i, j) not in lines and (j, i) not in lines:
                    min_dist = dist
                    nearest_index = j
        if nearest_index != -1:
            lines.append((i, nearest_index))
    return lines

def get_user_points():
    points = []
    while True:
        user_input = input("Enter a point (x, y) or type 'done' to finish: ")
        if user_input.lower() == 'done':
            break
        try:
            x, y = map(float, user_input.split(','))
            points.append((x, y))
        except:
            print("Invalid input. Please enter coordinates in the format 'x, y'.")
    return points

# Function to plot points and lines
def plot_points_and_lines(points, lines):
    # Plot the points
    x_coords = [p[0] for p in points]
    y_coords = [p[1] for p in points]
    plt.scatter(x_coords, y_coords, color='blue', label='Points')
    

    for (i, j) in lines:
        plt.plot([points[i][0], points[j][0]], [points[i][1], points[j][1]], color='red')
   
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.title('Points Connected to Nearest Neighbor')
    plt.legend()
    plt.grid(True)
    plt.show()
if __name__ == "__main__":
    points = get_user_points()
 


    if len(points) < 2:
        print("At least two points are required to draw lines.")
    else:
     
        lines = find_nearest_neighbor(points)
        plot_points_and_lines(points, lines)
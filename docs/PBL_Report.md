# PROJECT BASED LEARNING (PBL) REPORT
## Smart City Route Optimization and Intelligent Navigation System

**Subject:** Data Structures and Algorithms  
**Submitted by:** ______________________________  
**Roll Number:** ______________________________  
**Semester / Section:** ______________________________  
**Submitted to:** ______________________________  
**Department:** ______________________________  
**Institution:** ______________________________  
**Academic Year:** ______________________________  

---

## Abstract
This project demonstrates how graph algorithms can be used to find routes in a small fictional city. Locations are represented as vertices and roads as weighted edges. The application is written in C and uses GTK4 for a desktop interface. A user can choose a starting location, destination, and algorithm. Dijkstra and A* search use road weights, while BFS and DFS demonstrate unweighted graph traversal. A traffic checkbox applies a small, fixed penalty to selected roads so that the effect of changing edge weights can be observed. The interface displays the route, its reported cost, and the number of visited nodes. The project is an educational prototype; it does not use live maps, GPS, or traffic data.

## 1. Introduction
Route planning is a useful example of graph-based problem solving. A city can be represented as a graph in which intersections or landmarks are vertices and roads are edges. If each road has a cost, a weighted graph algorithm can find a low-cost route. This project provides a small, visual demonstration of these ideas using a fictional road network.

## 2. Problem Statement
Students often learn graph algorithms through numerical examples, but it can be difficult to connect the steps of an algorithm to a route. The project addresses this by providing an interactive city graph and allowing users to compare different graph-search methods on the same data.

## 3. Objectives
- Represent a small road network as a graph.
- Implement Dijkstra, A*, BFS, and DFS in C.
- Allow source, destination, and algorithm selection through a desktop interface.
- Demonstrate the effect of changing selected road weights.
- Display route information and a basic visited-node count.
- Provide a project that can be compiled, demonstrated, and explained in a DAA viva.

## 4. Scope and Limitations
The project uses seven fictional locations and fixed sample road distances. The traffic switch adds a fixed penalty to two roads; it is not connected to a live traffic service. The graph is small and stored in source code. There is no GPS, map tile service, user account, or route database. A* uses straight-line coordinates as a demonstration heuristic; because the coordinates and road weights are hand-entered and not calibrated, the heuristic is not guaranteed to preserve optimality for this sample.

## 5. System Requirements
**Hardware:** A basic computer capable of running a C compiler and GTK4 desktop application.  
**Software:** C11 compiler, GTK4 development files, Cairo (normally included with GTK4), `pkg-config`, and Make.  
**Operating system:** Linux or Windows using MSYS2 UCRT64; macOS may work with GTK4 installed through a package manager.

## 6. System Design
The graph is stored as an adjacency matrix. A non-zero matrix entry represents a road and its base distance in kilometres. The application has four main responsibilities:
1. Read the selected source, destination, and algorithm from the GUI.
2. Run the chosen algorithm on the graph.
3. build a path from predecessor information.
4. Display the route and highlight it on the drawing area.

See `diagrams.md` for editable flowchart and module diagrams.

## 7. Algorithms Used
### Dijkstra’s algorithm
Dijkstra repeatedly selects the unvisited vertex with the smallest known distance and relaxes its outgoing edges. It is used to find the minimum weighted distance when edge weights are non-negative. In this project, the traffic option modifies selected edge weights before the search.

### A* search
A* ranks a candidate using `f(n) = g(n) + h(n)`, where `g(n)` is the cost so far and `h(n)` estimates the remaining cost. The project uses straight-line distance between sample coordinates as its illustrative heuristic. Since the road weights are arbitrary sample values, the heuristic is not guaranteed to be admissible; therefore the program should not claim A* always returns the optimal route on this dataset.

### Breadth-First Search (BFS)
BFS explores vertices level by level using a queue. In an unweighted graph it finds a path with the fewest edges. The GUI reports the number of roads in the path rather than a distance in kilometres.

### Depth-First Search (DFS)
DFS explores one branch before backtracking, using a stack. It is included to demonstrate graph traversal. The route it discovers depends on neighbour order and is not guaranteed to minimize distance or edge count.

## 8. Implementation
The application is written in C. The graph and location names are defined in `src/main.c`; the same road list is provided as a readable CSV file. Algorithm functions return a `RouteResult` structure containing the path, path length, reported cost, visited-node count, and a found/not-found flag. GTK4 provides the window, controls, result labels, and drawing area. Cairo draws roads, labels, vertices, and the selected route.

## 9. Testing Plan
Manual test cases are provided in `tests/manual_test_cases.md`. They cover a weighted route, algorithm selection, traffic changes, same-source/destination behavior, and interface validation. These are expected outcomes, not evidence of execution. Record actual results and screenshots after compiling and running the program.

## 10. Results and Discussion
The expected demonstration is that the same city graph can produce different route choices depending on the algorithm and edge weights. Dijkstra optimizes total road cost; BFS optimizes the number of edges; DFS demonstrates traversal; A* prioritizes nodes using both known cost and a heuristic estimate. Visited-node counts are included as a simple comparison metric, but they do not establish a universal speed ranking.

## 11. Conclusion
The project connects core graph concepts to a small interactive application. It demonstrates weighted and unweighted search, predecessor-based path reconstruction, and the effect of modifying edge costs. The project is intended for learning and classroom demonstration rather than real navigation.

## 12. Future Scope
- Load road data from an external file instead of compiling it into the program.
- Add a user-editable road/weight panel.
- Use a validated heuristic and include a guaranteed-optimal A* configuration.
- Add more locations and compare adjacency-list and adjacency-matrix representations.
- Record algorithm run time over larger generated graphs.
- Integrate a real map or traffic feed only as a separate, carefully tested extension.

## References
1. Cormen, Leiserson, Rivest, and Stein, *Introduction to Algorithms*, graph algorithms chapters.
2. GTK Project, GTK4 documentation: https://docs.gtk.org/gtk4/
3. Cairo graphics library documentation: https://www.cairographics.org/documentation/
4. Mermaid documentation: https://mermaid.js.org/

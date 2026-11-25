# Manual test cases (expected outcomes, not executed test results)

The graph and weights are fictional. Run these cases after compiling and record actual output/screenshots for your report.

| ID | Inputs | Expected outcome |
|---|---|---|
| T1 | Central Station → Airport Road; Dijkstra; traffic off | Route found; shortest weighted distance is 11 km: Central Station → City Hospital → University → Airport Road. |
| T2 | Central Station → Airport Road; A*; traffic off | Route found; with the provided heuristic it should return a weighted route; compare its cost with Dijkstra. If costs differ, investigate heuristic admissibility/consistency before claiming optimality. |
| T3 | Central Station → Airport Road; BFS | Route with the fewest number of edges; result reports roads used, not km. |
| T4 | Central Station → Airport Road; DFS | A route/traversal is displayed; do not expect it to be shortest. |
| T5 | Same Dijkstra query with traffic on | A route is found; costs on the two designated roads are increased by 5 km each, which may change the selected route. |
| T6 | Same source and destination | One-location route, cost 0 for weighted algorithms and 0 roads for BFS/DFS. |
| T7 | Select valid locations and change algorithms repeatedly | UI remains responsive and result text updates. |
| T8 | Try invalid or unselected controls if possible | Program displays a selection prompt rather than crashing. |

## Note on A*
The example uses straight-line coordinate distance as its heuristic, while edge weights are hand-entered values. Those values are not guaranteed to be greater than geometric distance or to be calibrated to it. Therefore the heuristic is illustrative, and A* optimality is not guaranteed by this dataset. For a strict optimality demonstration, use a heuristic scaled to a proven lower bound (or set h=0, which reduces A* to Dijkstra).

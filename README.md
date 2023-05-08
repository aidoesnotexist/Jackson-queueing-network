# Jackson Queueing Network

This program simulates the life cycle of one open (Jackson) queueing network.
Four nodes, external arrivals form a Poisson stream/process to each node.
Each node represents a M|M|1 system.
Service time is exponentially distributed.

Program finds:
-stationary probability of a queue size in the certain node
-maximum number of customers to arrive
-average time of a job going through the system (waiting and serving) depending on the node it originally came in

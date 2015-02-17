#include<stdio.h>
#include<stdlib.h>
#include <vector>
#include <algorithm>

using namespace std;

typedef struct node{
        int num_block;
		int out_degree;
        struct node *next;
}block;

block *block_graph,*p,*l;

typedef struct dd_node{
        int num_block;
		int in_ddGraph;
		int *path;
		int path_length;
        struct dd_node *next;
}dd_block;

dd_block *dd_graph, *reverse_dd_graph, *pdd, *ldd;

int read_line(char *buffer, size_t max, FILE *in) {
	return fgets(buffer, max, in) == buffer;
}

void printAdjacencyListBlockGraph(int n, block* block_graph) {
	printf("\n#### Printing the Adjacency List of the Block Graph ####\n\n");
	for (int i=1; i<=n; i++){
        printf("%d->",block_graph[i].num_block);
        l = block_graph[i].next;
        while (l != NULL) {
              printf("%d->",l->num_block);
              l = l->next;
        }
        printf("NULL, out_degree = %d\n",block_graph[i].out_degree);
    }
	printf("\n");

}

void printAdjacencyListDDGraph(int n, dd_block *dd_graph) {
	printf("\n#### Printing the Adjacency List of the dd-Graph (with the paths from the Block Graph) ####\n\n");
	for (int i=1; i<=n; i++) {
		if (dd_graph[i].in_ddGraph == 1) {
			//printf("i: %d\n",i);
			printf("%d->",dd_graph[i].num_block);
			pdd = dd_graph[i].next;
			while (pdd != NULL) {
				printf("%d { ",pdd->num_block); //print the path to that also!
				for (int j=1; j <= pdd->path_length; j++){
						printf("%d ", pdd->path[j]);
				}
				printf("} ->");
				pdd = pdd->next;
			}
			printf("NULL\n");
		}
	}
}

void printAdjacencyListReverseDDGraph(int n, dd_block *reverse_dd_graph) {
	printf("\n#### Printing the Adjacency List of the Reverse dd-Graph (with the non-reverted(!) paths from the Block Graph) ####\n\n");
	for (int i=1; i<=n; i++) {
		if (reverse_dd_graph[i].in_ddGraph == 1) {
			//printf("i: %d\n",i);
			printf("%d->",reverse_dd_graph[i].num_block);
			pdd = reverse_dd_graph[i].next;
			while (pdd != NULL) {
				printf("%d { ",pdd->num_block); //print the path to that also!
				for (int j=1; j <= pdd->path_length; j++){
						printf("%d ", pdd->path[j]);
				}
				printf("} ->");
				pdd = pdd->next;
			}
			printf("NULL\n");
		}
	}
}

void print_vector_3d(vector< vector< vector <int> > > pathaki_3d){
	//printf("\n#### Printing a 3d vector! ####\n");
	for (std::vector<std::vector<std::vector<int> > >::iterator it2 = pathaki_3d.begin(); it2 != pathaki_3d.end(); ++it2) {
		if (!((*it2).empty())) { // print only if the vector of vectors is not empty!
		for (std::vector<std::vector<int> >::iterator it1 = (*it2).begin(); it1 != (*it2).end(); ++it1) {
			for (std::vector<int>::iterator it0 = (*it1).begin(); it0 != (*it1).end(); ++it0) {
				printf("%d ", *it0);
			}
			if (it1 < (*it2).end()-1) printf("| ");
		}
		printf("\n");
		}
	}

}

void print_vector_2d(vector< vector <int> > vec_2d) {
	printf("\n");
	for (std::vector<std::vector<int> >::iterator it1 = vec_2d.begin(); it1 != vec_2d.end(); ++it1) {
			for (std::vector<int>::iterator it0 = (*it1).begin(); it0 != (*it1).end(); ++it0) {
				printf("%d ", *it0);
			}
			printf("\n");
	}
}

void print_vector(vector <int> vec){
	for (std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it) {
				printf("%d ", *it);
			}
}

void writeResultsToFile(int total_paths_excluded, int paths, int forwardTrees, int backwardTrees, vector< vector< vector <int> > > vec_3d){
	int count, counter, counter2;
	FILE *f;
	f = fopen("results.txt","w"); //SOS: write to results.txt file!
	
	fprintf(f,"Hennel's Generalized Algorithm results\n\n");
	fprintf(f,"Number of paths generated by the algorithm: %d\n", paths);
	fprintf(f,"Number of Forward Trees: %d\n", forwardTrees);
	fprintf(f,"Number of Backward Trees: %d\n", backwardTrees);
	fprintf(f,"Number of solutions/combinations (with duplicates): %d\n", forwardTrees * backwardTrees);
	
	count = 0;
	for (std::vector<std::vector<std::vector<int> > >::iterator it2 = vec_3d.begin(); it2 != vec_3d.end(); ++it2)
		if (!((*it2).empty())) count++;
	
	fprintf(f,"Number of solutions/combinations (without the duplicates): %d\n", count);
	fprintf(f,"Number of total paths excluded: %d\n", total_paths_excluded);
	
	counter = 0;
	for (std::vector<std::vector<std::vector<int> > >::iterator it2 = vec_3d.begin(); it2 != vec_3d.end(); ++it2) {
		if (!((*it2).empty())) {
			counter++;
			fprintf(f,"\nSolution paths No %d:\n", counter);
			counter2 = 0;
			for (std::vector<std::vector<int> >::iterator it1 = (*it2).begin(); it1 != (*it2).end(); ++it1) {
				if (!((*it1).empty())) {
					counter2++;
					fprintf(f,"%d) ", counter2);
					for (std::vector<int>::iterator it0 = (*it1).begin(); it0 != (*it1).end(); ++it0) {
						fprintf(f, "%d ", *it0);
					}
					if (it1 < (*it2).end()-1) fprintf(f,"\n");
				}
			}
			fprintf(f,"\n");
		}
	}
	
}


int main () {
	
	int numberOfBlocks, i, counter, start, bl, finish, thesi, k, readblock, element, neighboor, progonos, front, back;
	int found, size, is_first, prev, last_element, last, times, apogonos, paths, numberOfExcludedPaths;
	int multiples, numberForward = 1, numberBackward = 1; 	// we assume that exists at least one SP-tree (the dd-graph is connected, 
															//e.g. i can reach from the first block ("1") all the others!
	char line[256], *beg, *end;
	
	vector<int> construct_path, queue, neighboors_list, pathaki, pathaki_1d, first_vector;
	vector< vector <int> > pathaki_2d, pathaki_2d_fw, paths_help_2d, excluded_paths;
	vector< vector< vector <int> > > pathaki_3d; // edges not in the forward tree and all the multiple ones that can be or not (at least one of them can be!)
	vector< vector< vector <int> > > predecessors_forward_paths; //All possible edges in the forward tree
	vector< vector< vector <int> > > result_paths, result_paths_final, result_paths_final_after_removal_of_duplicates; // this should hold the solution paths of the Hennel algorithm
	vector< vector< vector <int> > > extracted_paths; // to create all the possible combinations for the result_paths!
	vector< vector< vector <int> > > apogonoi_backward; //All possible edges/paths in Backward tree
	vector< vector< vector <int> > > apogonoi_backward_path_comb; // All possible combinations of the edges/paths of all Backward trees
	
	int *explored_forward, *explored_backward, *multiple, *multiple_bw, *predecessors_forward, *apogonoi;
	
	read_line(line, sizeof(line), stdin);
	numberOfBlocks = atoi (line);
	int first_block = 1; // always!
	int last_block = numberOfBlocks; // always!
	block_graph = (block *) malloc ((numberOfBlocks+1) * sizeof(block));
	
	// Make the Adjacency list 
	for (i = 1; i <= numberOfBlocks; i++) {
		block_graph[i].num_block = i;
		block_graph[i].out_degree = 0;
		block_graph[i].next = NULL;
	}
	
	for (i = 1; i < numberOfBlocks; i++) { // the last node is a sink node always!
		l = &(block_graph[i]);		
		read_line(line, sizeof(line), stdin);
		counter = -1;
		beg = line;
        for (beg = line; ; beg = end) {
            readblock = strtol(beg, &end, 10);
            if (beg == end) break;
			counter++;
			// adjacency list making: add the neighboor block at the end of the list!!!
			if (readblock != i) {
				while (l->next != NULL) { l = l->next; };
				p = (block *) malloc (sizeof(block));
				p->num_block = readblock;
				p->out_degree = -1;
				p->next = NULL;
				l->next = p;
			}
        }
		block_graph[i].out_degree = counter;
	}
	
	// print the adjacency list for debbuging
	printAdjacencyListBlockGraph(numberOfBlocks,block_graph);
	
	// Make the DD-graph
	dd_graph = (dd_block *) malloc ((numberOfBlocks+1) * sizeof(dd_block));

	for (i = 1; i <= numberOfBlocks; i++) {
		dd_graph[i].num_block = i;
		dd_graph[i].in_ddGraph = 1; // all nodes of the block graph in the dd_graph at start!
		dd_graph[i].path_length = 0;
		dd_graph[i].next = NULL;
	}
	
	for (i = 1; i < numberOfBlocks; i++) {
		//printf("i : %d\n", i);
		if (dd_graph[i].in_ddGraph == 0) continue;
		
		start = i;
		for (l = block_graph[i].next; l != NULL; l = l->next){
			construct_path.push_back(start);
			bl = l->num_block;
			while ( (block_graph[bl].out_degree == 1)  && (bl != start) ) { // make the paths!
				construct_path.push_back(bl);
				dd_graph[bl].in_ddGraph = 0;
				bl = (block_graph[bl].next)->num_block;
			}
			finish = bl;
			if ( construct_path.back() != finish ) construct_path.push_back(finish);
			
			// make the ddGraph node/block to add:
			pdd = (dd_block *) malloc (sizeof(dd_block));
			pdd->num_block = finish;
			pdd->in_ddGraph = 1;
			pdd->path_length = construct_path.size();
			pdd->path = (int *) malloc((pdd->path_length + 1) * sizeof(int));
			
			for(std::vector<int>::size_type j = 0; j != construct_path.size(); j++) {
				thesi = j+1;
				pdd->path[thesi] = construct_path[j];
			}
			//for (k = 1; k <= pdd->path_length; k++) printf("%d ",pdd->path[k]);
			
			construct_path.clear();
			pdd->next = dd_graph[start].next;
			dd_graph[start].next = pdd;
			
		}
		
	}
	dd_graph[last_block].in_ddGraph = 1; // the sink node is always in the dd_graph!
	
	// print the adjacency list for debbuging
	printAdjacencyListDDGraph(numberOfBlocks,dd_graph);
	
	/* ################################################# */
	// BFS ston dd_graph (forward tree of the Hennel algorithm)
	explored_forward = (int *) malloc ((numberOfBlocks+1) * sizeof(int));
	multiple = (int *) malloc ((numberOfBlocks+1) * sizeof(int));
	predecessors_forward = (int *) malloc ((numberOfBlocks+1) * sizeof(int));
	
	for (i = 1; i <= numberOfBlocks; i++) {
		explored_forward[i] = 0; // all nodes start unexplored
		multiple[i] = 1; // no multiples at start (1 means solo edges)
		predecessors_forward[i] = 0;
	}
	
	// Push initial vertex to the queue and mark it as explored! Always "1" is the first block/node of the programm!
	queue.push_back(1);
	explored_forward[1] = 1;
	//printf("\nBFS Explored nodes:\n");
	while (!queue.empty()) { // still elements to search!
		element = queue[0];
		queue.erase( queue.begin() ); // dequeue the element
		//printf("%d ",element);
		
		// start searching its neighboors! (no neighboors at start)
		ldd = &(dd_graph[element]);
		neighboors_list.clear();
		while (ldd->next != NULL){
			neighboor = (ldd->next)->num_block;
			
			// check for multi edges...
			for (std::vector<int>::iterator it = neighboors_list.begin(); it != neighboors_list.end(); ++it) {
				if ( *it == neighboor ) {
					multiple[element]++;
				}
			}
			neighboors_list.push_back(neighboor);
			
			if (!explored_forward[neighboor]) {
				queue.push_back(neighboor);
				explored_forward[neighboor] = 1;
				predecessors_forward[neighboor] = element;
			} else { // all edges (element->neighboor) that don't belong to the forward tree (or are multiples of the ones that belong!)
				
				for (int j=1; j <= (ldd->next)->path_length; j++){
						pathaki.push_back((ldd->next)->path[j]);
						//printf("%d ", (ldd->next)->path[j]);
				}
				//for (std::vector<int>::iterator it = pathaki.begin(); it != pathaki.end(); ++it) { printf("%d ", *it); } printf("\n");
				pathaki_2d.push_back(pathaki);
				pathaki.clear();
				pathaki_3d.push_back(pathaki_2d);
				pathaki_2d.clear();
			}
			ldd = ldd->next;
		}
		
	}
	
	printf("\nPredecessors table:\n");
	for (i=1; i<=numberOfBlocks; i++) {
		progonos = predecessors_forward[i];
		if (progonos != 0) { // belongs to the tree and we wanna put all the edges even the multiple ones
			// on the 3d vector we are building with the tree paths!
			printf("\n%d paidi tou %d", i, progonos);
			ldd = &(dd_graph[progonos]);
			while (ldd->next != NULL){
				neighboor = (ldd->next)->num_block;
				if (neighboor == i) {
					for (int j=1; j <= (ldd->next)->path_length; j++){
						pathaki.push_back((ldd->next)->path[j]);
						//printf("%d ", (ldd->next)->path[j]);
					}
					pathaki_2d.push_back(pathaki);
					pathaki.clear();
				}
				ldd = ldd->next;
			}
			predecessors_forward_paths.push_back(pathaki_2d);
			pathaki_2d.clear();
		}
			
	}
	
	printf("\n\nMultiple edges in DD-Graph:\n\n");
	for (i=1; i<=numberOfBlocks; i++) {
		if (multiple[i] > 1) printf("O kombos %d exei %d multiple edges\n", i, multiple[i]);
	}
	
	printf("\nAll possible edges in the forward tree\n");
	print_vector_3d(predecessors_forward_paths);
	
	// count the number of possible forward trees:
	for (std::vector<std::vector<std::vector<int> > >::iterator it1 = predecessors_forward_paths.begin(); it1 != predecessors_forward_paths.end(); ++it1) {
		multiples = (*it1).size();
		if ( multiples > 1 ) numberForward = numberForward * multiples;
	}
	printf("\nThere are %d forward trees\n", numberForward);
	
	// now we need to add to the pathaki_3d the edges that are in the forward tree and are multiple!
	for (std::vector<std::vector<std::vector<int> > >::iterator it1 = pathaki_3d.begin(); it1 != pathaki_3d.end(); ++it1) {
		first_vector.clear();
		first_vector = (*it1)[0];
		front = first_vector.front();
		back = first_vector.back();
		if ( (predecessors_forward[back] == front) /* belongs to the forward tree */ && (multiple[front] > 1)/* is multiple */ ) {
			//printf("%d->%d\n", front, back);
			ldd = &(dd_graph[front]);
			while (ldd->next != NULL){
				neighboor = (ldd->next)->num_block;
				if ( neighboor == back ) {
					for (int j=1; j <= (ldd->next)->path_length; j++){
						pathaki.push_back((ldd->next)->path[j]);
						//printf("%d ", (ldd->next)->path[j]);
					}
					// balto sto pathaki_3d an den iparxei idi!
					size = (*it1).size();
					found = 0;
					for (size_t j=0; j < size; j++){
						if ( pathaki == (*it1)[j] ) found = 1;
					}
					if (found == 0) {
						(*it1).push_back(pathaki);
						size++;
					}
					pathaki.clear();
				}
				
				ldd = ldd->next;
			}
		}
	}
	
	
	printf("\nPaths not in the forward tree (from the BFS) and all the multiple ones that can be or not\n");
	print_vector_3d(pathaki_3d);
	
	paths = pathaki_3d.size() + 1; 
	printf("\nNumber of paths generated by Hennel algorithm: %d\n", paths); 
	
	// make all the possible combinations of the "mid-edges" of the Hennel algorithm
	// so as to start building the paths
	
	printf("\nAll possible combinations of the 'mid-edges' of the Hennel algorithm\n\n");
	is_first = 0;
	for (std::vector<std::vector<std::vector<int> > >::iterator it1 = pathaki_3d.begin(); it1 != pathaki_3d.end(); ++it1) {
		for (std::vector<std::vector<int> >::iterator it2 = (*it1).begin(); it2 != (*it1).end(); ++it2) {
			// put the vector to all in the result_paths
			pathaki = *it2;
			is_first++;
			// unless of course the result_paths are empty!
			if ( result_paths.begin() == result_paths.end() ) {
				pathaki_2d.push_back(pathaki);
				result_paths.push_back(pathaki_2d); //put something in there!
				pathaki_2d.clear();
				pathaki.clear();
				continue;
			}
			if (is_first == 1) {
				for (std::vector<std::vector<std::vector<int> > >::iterator it3 = result_paths.begin(); it3 != result_paths.end(); ++it3) {
					(*it3).push_back(pathaki);
				}
				pathaki.clear();
			} else { //multiple edges
				for (std::vector<std::vector<std::vector<int> > >::iterator it3 = result_paths.begin(); it3 != result_paths.end(); ++it3) {
					extracted_paths.push_back(*it3); // extract the paths already added
				}
				
				for (std::vector<std::vector<std::vector<int> > >::iterator it3 = extracted_paths.begin(); it3 != extracted_paths.end(); ++it3) {
					(*it3).pop_back(); // remove the last element (one multiple edge)
					(*it3).push_back(pathaki); // put the other multiple edge
					result_paths.push_back(*it3); // put it back anew!
				}
				pathaki.clear();
				extracted_paths.clear();
				
			}
		}
		is_first = 0;
	}
	
	print_vector_3d(result_paths);
	
	// Add the shortest path from the first_block (1) to the last_block (numberOfBlocks) from the forward tree for every result_path
	for (std::vector<std::vector<std::vector<int> > >::iterator it1 = result_paths.begin(); it1 != result_paths.end(); ++it1) {
		pathaki_2d.clear();
		pathaki_2d = (*it1);
		last = last_block;
		pathaki.clear(); // pathaki will have the (first_block,...,last_block) path in end!
		prev = predecessors_forward[last];
		while (prev != 0) { // 0 -> 1 (first_block) in the predecessors_forward table
			for (std::vector<std::vector<std::vector<int> > >::iterator it2 = predecessors_forward_paths.begin(); it2 != predecessors_forward_paths.end(); ++it2) {
				pathaki_2d_fw.clear();
				pathaki_2d_fw = (*it2);
				last_element = pathaki_2d_fw[0].back(); // last element of the first vector! 
				if (last_element == last) {
					// no path from the forward tree paths should be the same as the one in the mid-edges of the algorithm (result_paths as is now!)
					for (std::vector<std::vector<int> >::iterator it3 = pathaki_2d_fw.begin(); it3 != pathaki_2d_fw.end(); ++it3) {
						pathaki_1d.clear();
						pathaki_1d = (*it3);
						counter = 0;
						for (std::vector<std::vector<int> >::iterator it4 = pathaki_2d.begin(); it4 != pathaki_2d.end(); ++it4) {
							if ( pathaki_1d != (*it4) ) counter++;
						}
						if ( counter == pathaki_2d.size() ) break;
					}
					break;
				} else continue;
			}
			pathaki_1d.erase( pathaki_1d.begin() ); // remove the first element (it is added as last from the next round!)
			pathaki.insert(pathaki.begin(), pathaki_1d.begin(), pathaki_1d.end()); // [pathaki | pathaki_1d]
			last = prev; // go to the next one backwards!
			prev = predecessors_forward[last];
		}
		pathaki.insert(pathaki.begin(), first_block); // insert the first_block in the start of the path(value = 1)
		//printf("\nPrint:"); print_vector(pathaki);
		(*it1).push_back(pathaki);
	}
	
	printf("\nAll possible combinations of the 'mid-edges' of the Hennel algorithm with the shortest path from start_block to last_block\n\n");
	print_vector_3d(result_paths);
	
	// connect the paths from the forward tree
	for (std::vector<std::vector<std::vector<int> > >::iterator it1 = result_paths.begin(); it1 != result_paths.end(); ++it1) {
		pathaki_2d.clear();
		pathaki_2d = (*it1); // copy them so i should know later which ones i don't want to be in the paths!
		for (std::vector<std::vector<int> >::iterator it5 = (*it1).begin(); it5 != (*it1).end()-1;/* don't change the shortest path! */ ++it5) {
			pathaki.clear();
			pathaki = (*it5); // this needs to be added the path: [1,...,| pathaki] from the forward tree
			last = pathaki[0];
			prev = predecessors_forward[last];
			times = 0;
			while (prev != 0) { // 0 -> 1 (first_block) in the predecessors_forward table
				for (std::vector<std::vector<std::vector<int> > >::iterator it2 = predecessors_forward_paths.begin(); it2 != predecessors_forward_paths.end(); ++it2) {
					pathaki_2d_fw.clear();
					pathaki_2d_fw = (*it2);
					last_element = pathaki_2d_fw[0].back(); // last element of the first vector! 
					if (last_element == last) {
						// no path from the forward tree paths should be the same as the one in the mid-edges of the algorithm (result_paths as is now!)
						for (std::vector<std::vector<int> >::iterator it3 = pathaki_2d_fw.begin(); it3 != pathaki_2d_fw.end(); ++it3) {
							pathaki_1d.clear();
							pathaki_1d = (*it3);
							counter = 0;
							for (std::vector<std::vector<int> >::iterator it4 = pathaki_2d.begin(); it4 != pathaki_2d.end(); ++it4) {
								if ( pathaki_1d != (*it4) ) counter++;
							}
							if ( counter == pathaki_2d.size() ) break;
						}
						break;
					} else continue;
				}
				pathaki_1d.erase( pathaki_1d.begin() ); // remove the first element (it is added as last from the next round!)
				times++;
				if (times == 1) pathaki.insert(pathaki.begin(), pathaki_1d.begin(), pathaki_1d.end()-1); // [pathaki_1d | pathaki] xoris to duplicate ending!
				else pathaki.insert(pathaki.begin(), pathaki_1d.begin(), pathaki_1d.end());
				last = prev; // go to the next one backwards!
				prev = predecessors_forward[last];
			}
			pathaki.insert(pathaki.begin(), first_block); // insert the first_block in the start of the path(value = 1)
			//printf("\n"); print_vector(pathaki);
			(*it5) = pathaki;
		}
	}
	
	printf("\nHennel paths from all forward trees + shortest from start_block to last_block\n\n");
	print_vector_3d(result_paths);
	
	// make the adjacency list for the Reverse DD-graph
	reverse_dd_graph = (dd_block *) malloc ((numberOfBlocks+1) * sizeof(dd_block));

	for (i = 1; i <= numberOfBlocks; i++) {
		reverse_dd_graph[i].num_block = i;
		reverse_dd_graph[i].in_ddGraph = 0;
		reverse_dd_graph[i].path_length = 0;
		reverse_dd_graph[i].next = NULL;
	}
	
	for (i = 1; i < numberOfBlocks; i++) {
		ldd = &(dd_graph[i]);
		if (ldd->in_ddGraph == 1) { // reverse the blocks!
			element = ldd->num_block;
			while (ldd->next != NULL) {
				neighboor = (ldd->next)->num_block;
				// make the reverse_dd_graph node/block
				pdd = (dd_block *) malloc (sizeof(dd_block));
				pdd->num_block = element;
				pdd->in_ddGraph = 1;
				pdd->path_length = (ldd->next)->path_length;
				pdd->path = (int *) malloc((pdd->path_length + 1) * sizeof(int));
				for (int j=1; j <= pdd->path_length; j++) {
					pdd->path[j] = (ldd->next)->path[j];
				}
				// now add it:
				reverse_dd_graph[neighboor].in_ddGraph = 1;
				pdd->next = reverse_dd_graph[neighboor].next;
				reverse_dd_graph[neighboor].next = pdd;
				ldd = ldd->next;
			}
		}
	}
	reverse_dd_graph[first_block].in_ddGraph = 1; // the first node (1) always on the reverse DD-graph
	
	printAdjacencyListDDGraph(numberOfBlocks,dd_graph);
	printAdjacencyListReverseDDGraph(numberOfBlocks, reverse_dd_graph);
	
	
	/* ################################################# */
	/* ################################################# */
	// BFS ston reverse_dd_graph (backward tree of the Hennel algorithm)
	
	explored_backward = (int *) malloc ((numberOfBlocks+1) * sizeof(int));
	multiple_bw = (int *) malloc ((numberOfBlocks+1) * sizeof(int));
	apogonoi = (int *) malloc ((numberOfBlocks+1) * sizeof(int));
	
	for (i = 1; i <= numberOfBlocks; i++) {
		explored_backward[i] = 0; // all nodes start unexplored
		multiple_bw[i] = 1; // no multiples at start (1 means solo edges)
		apogonoi[i] = 0;
	}
	
	// Push initial vertex to the queue and mark it as explored! 
	// For the backward tree choose the last_block of the programm!
	queue.clear();
	queue.push_back(last_block);
	explored_forward[1] = 1;
	//printf("\nBFS (Backward tree) - Explored nodes:\n");
	while (!queue.empty()) { // still elements to search!
		element = queue[0];
		queue.erase( queue.begin() ); // dequeue the (first) element
		//printf("%d ",element);
		
		// start searching its neighboors! (no neighboors at start)
		ldd = &(reverse_dd_graph[element]);
		neighboors_list.clear();
		while (ldd->next != NULL){
			neighboor = (ldd->next)->num_block;
			
			// check for multi edges...
			for (std::vector<int>::iterator it = neighboors_list.begin(); it != neighboors_list.end(); ++it) {
				if ( *it == neighboor ) {
					multiple_bw[element]++;
				}
			}
			neighboors_list.push_back(neighboor);
			
			if (!explored_backward[neighboor]) {
				queue.push_back(neighboor);
				explored_backward[neighboor] = 1;
				apogonoi[neighboor] = element;
			}
			ldd = ldd->next;
		}
		
	}
	
	printf("\nMultiple edges in Reverse DD-Graph:\n\n");
	for (i=1; i<=numberOfBlocks; i++) {
		if (multiple_bw[i] > 1) printf("O kombos %d exei %d multiple edges\n", i, multiple_bw[i]);
	}
	
	printf("\nPinakas apogonon in Backward tree:\n\n"); // calculate the number of Backward trees also!
	for (i=1; i<=numberOfBlocks; i++) {
		if (apogonoi[i] != 0) printf("O apogonos tou %d einai o %d\n", i, apogonoi[i]);
		if ( (apogonoi[i] != 0) && (multiple[i] > 1) ) numberBackward = numberBackward * multiple[i];
	}
	printf("\nThere are %d backward trees\n", numberBackward);
	
	
	// build the backward edges/paths with the multiple ones
	for (i=1; i<=numberOfBlocks; i++) {
		if (apogonoi[i] != 0) {
			apogonos = apogonoi[i];
			// carefull: to build the paths you search the dd_graph adjacency list!
			ldd = &(dd_graph[i]);
			pathaki_2d.clear();
			while (ldd->next != NULL) {
				if ( (ldd->next)->num_block == apogonos ){
					pathaki.clear();
					for (int j=1; j <= (ldd->next)->path_length; j++){
						pathaki.push_back((ldd->next)->path[j]);
					}
					pathaki_2d.push_back(pathaki);
				}
				ldd = ldd->next;
			}
			apogonoi_backward.push_back(pathaki_2d);
		}
	}
	
	printf("\nAll Backward tree edges/paths:\n\n");
	print_vector_3d(apogonoi_backward);
	
	
	
	// make the all the possible combinations of backward paths!!!
	pathaki_2d.clear();
	extracted_paths.clear();
	is_first = 0;
	for (std::vector<std::vector<std::vector<int> > >::iterator it1 = apogonoi_backward.begin(); it1 != apogonoi_backward.end(); ++it1) {
		for (std::vector<std::vector<int> >::iterator it2 = (*it1).begin(); it2 != (*it1).end(); ++it2) {
			// put the vector to all in the apogonoi_backward_path_comb
			pathaki = *it2;
			is_first++;
			// unless of course the apogonoi_backward_path_comb are empty!
			if ( apogonoi_backward_path_comb.begin() == apogonoi_backward_path_comb.end() ) {
				pathaki_2d.push_back(pathaki);
				apogonoi_backward_path_comb.push_back(pathaki_2d); //put something in there!
				pathaki_2d.clear();
				pathaki.clear();
				continue;
			}
			if (is_first == 1) {
				for (std::vector<std::vector<std::vector<int> > >::iterator it3 = apogonoi_backward_path_comb.begin(); it3 != apogonoi_backward_path_comb.end(); ++it3) {
					(*it3).push_back(pathaki);
				}
				pathaki.clear();
			} else { //multiple edges
				for (std::vector<std::vector<std::vector<int> > >::iterator it3 = apogonoi_backward_path_comb.begin(); it3 != apogonoi_backward_path_comb.end(); ++it3) {
					extracted_paths.push_back(*it3); // extract the paths already added
				}
				
				for (std::vector<std::vector<std::vector<int> > >::iterator it3 = extracted_paths.begin(); it3 != extracted_paths.end(); ++it3) {
					(*it3).pop_back(); // remove the last element (one multiple edge)
					(*it3).push_back(pathaki); // put the other multiple edge
					apogonoi_backward_path_comb.push_back(*it3); // put it back anew!
				}
				pathaki.clear();
				extracted_paths.clear();
				
			}
		}
		is_first = 0;
	}
	
	
	printf("\nAll possible combinations of the edges/paths of all Backward trees:\n\n");
	print_vector_3d(apogonoi_backward_path_comb);
	
	// connect the backward paths to the result_paths
	for (std::vector<std::vector<std::vector<int> > >::iterator it2 = apogonoi_backward_path_comb.begin(); it2 != apogonoi_backward_path_comb.end(); ++it2) {
		pathaki_2d.clear();
		pathaki_2d = (*it2);
		// per backward tree do:
		
		for (std::vector<std::vector<std::vector<int> > >::iterator it1 = result_paths.begin(); it1 != result_paths.end(); ++it1) {
			paths_help_2d.clear();
			for (std::vector<std::vector<int> >::iterator it5 = (*it1).begin(); it5 != (*it1).end(); ++it5) {
				pathaki.clear();
				pathaki = (*it5); // this needs to be added the path: [pathaki | ..., last_block] from the backward tree(s)
				front = pathaki.back();
				
				// start searching for "connected" paths to the last_block!
				while (front != last_block) {
					for (std::vector<std::vector<int> >::iterator it3 = pathaki_2d.begin(); it3 != pathaki_2d.end(); ++it3){	
						if ( (*it3)[0] == front ) {
							pathaki_1d.clear();
							pathaki_1d = (*it3);
							pathaki_1d.erase( pathaki_1d.begin() ); // remove the first element (it is the last of the previous path-edge)
							front = pathaki_1d.back();
							pathaki.insert(pathaki.end(), pathaki_1d.begin(), pathaki_1d.end()); // [pathaki | pathaki_1d]
							break;
						}
					}
				} // pathaki is now completed from start to last block (as a path)
				paths_help_2d.push_back(pathaki);
			}
			
			result_paths_final.push_back(paths_help_2d);
		}
		
		
	}
	
	printf("\nFinal number of groups of paths generated: %d\n", result_paths_final.size() );
	//writeResultsToFile(paths, numberForward, numberBackward, result_paths_final);
	//printf("\nFinal results:\n\n");
	//print_vector_3d(result_paths_final);
	
	/* EXTRA */
	// search for same groups of paths in the solution and 'clear' them!
	times = 0;
	for (std::vector<std::vector<std::vector<int> > >::iterator it1 = result_paths_final.begin(); it1 != result_paths_final.end()-1; ++it1) {
		for (std::vector<std::vector<std::vector<int> > >::iterator it2 = it1+1; it2 != result_paths_final.end(); ++it2) {
			if ( std::is_permutation ((*it1).begin(), (*it1).end(), (*it2).begin()) ) {
				times++;
				(*it2).clear(); // clear them (leave no elements there!)
			}
		}
	}
	//printf("\nNumber of identical groups of paths generated: %d\n\n", times );
	
	//printf("\nFinal results after removal of the duplicates:\n\n");
	//print_vector_3d(result_paths_final);
	
	
	// EXCLUDE PATHS FROM FILE excluded_paths.txt//
	// read the file with the paths you want to exclude from the solutions!
	FILE* file = fopen("excluded_paths.txt", "r");
	if (file != NULL) {
		printf("\nFile excluded_paths.txt exists\n");
		read_line(line, sizeof(line), file);
		numberOfExcludedPaths = atoi (line);
		//printf("%d\n", numberOfExcludedPaths);
		
		for (i = 1; i <= numberOfExcludedPaths; i++) {
			//printf("i: %d\n", i);
			pathaki_1d.clear();
			read_line(line, sizeof(line), file);
			beg = line;
			for (beg = line; ; beg = end) {
				readblock = strtol(beg, &end, 10);
				if (beg == end) break;
				pathaki_1d.push_back(readblock);
				//printf("%d ", readblock);
			}
			//printf("\n");
			excluded_paths.push_back(pathaki_1d);
		}
	} else {
		printf("\nFile excluded_paths.txt doesn't exist\n");
	}
	
	print_vector_2d(excluded_paths);
	
	times = 0;
	for (std::vector<std::vector<std::vector<int> > >::iterator it2 = result_paths_final.begin(); it2 != result_paths_final.end(); ++it2) {
		if (!((*it2).empty())) { // only if the vector of vectors is not empty!
			for (std::vector<std::vector<int> >::iterator it1 = (*it2).begin(); it1 != (*it2).end(); ++it1) {
				// (*it1) is the vector/path we wanna check if is the same as one from the excluded_paths! 
				for (std::vector<std::vector<int> >::iterator it3 = excluded_paths.begin(); it3 != excluded_paths.end(); ++it3) {
					if ( (*it1) == (*it3) ) {
						(*it1).clear();
						times++;
						break;
					}
				}
			}
		}
	}
	
	// write the results to file results.txt
	writeResultsToFile(times, paths, numberForward, numberBackward, result_paths_final);
	
	return 0;
}
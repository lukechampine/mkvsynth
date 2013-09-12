#include "bufferAllocation.h"

/******************************************************************************
 * Also see MkvsynthOutput and MkvsynthSemaphoreList and createInputBuffer    *
 * and MkvsynthMetaData                                                       *
 *                                                                            *
 * createOutputBuffer allocates an MkvsynthOutput and creates the first node  *
 * in the list of semphores that help the outputs coordinate with the inputs. *
 *                                                                            *
 * createInputBuffer is in charge of maintaining consistency, thus            *
 * createOutputBuffer only needs to allocate memory.                          *
 *                                                                            *
 * recentFrame and semaphores are lists that are explicitly assigned to a     *
 * single output in all cases. createOutputBuffer creates the first node for  *
 * both of these lists.                                                       *
 *                                                                            *
 * outputBreadth and filtersRemaining are both initialized to 0 because they  *
 * depend on the number of filters using this output as input. But since the  *
 * output has just been created, there are clearly no filters using it for    *
 * input, thus these variables should be initialized to 0.                    *
 *                                                                            *
 * *** I am not sure if the memory allocated here is ever free()'d        *** *
 *****************************************************************************/

MkvsynthOutput *createOutputBuffer() {
	MkvsynthOutput *output = malloc(sizeof(MkvsynthOutput));

	output->semaphores = malloc(sizeof(MkvsynthSemaphoreList));
	output->recentFrame = malloc(sizeof(MkvsynthFrame));
	output->metaData = malloc(sizeof(MkvsynthMetaData));

	output->outputBreadth = 0;
	output->recentFrame->filtersRemaining = 0;
	return output;
}

/******************************************************************************
 * Also see MkvsynthSempahporeList and MkvsynthOutput and MkvsynthMetaData    *
 *                                                                            *
 * An input buffer needs to coordinate with an output buffer to ensure that   *
 * frames are not deleted from the output buffer until all input buffers have *
 * seen and computed the necessary information.                               *
 *                                                                            *
 * Multiple inputs can use the same MkvsynthOutput (though not vice-versa).   *
 *                                                                            *
 * Each MkvsynthInput has a semaphore to track how large its personal buffer  *
 * is, to prevent the previous filter from outputting frames when there is no *
 * room in memory for them. When the input buffer is being created, a new     *
 * semaphore needs to be added to the list for the particular input being     *
 * created.                                                                   *
 *                                                                            *
 * Similarly, the output needs to know how many filters are using it as input *
 * so output->outputBreadth must be increased by 1.                           *
 *****************************************************************************/
MkvsynthInput *createInputBuffer(MkvsynthOutput *output) {
	MkvsynthInput *input = malloc(sizeof(MkvsynthInput));
	
	int i;
	MkvsynthSemaphoreList *tmp = output->semaphores;

	// this may be replaced by a single variable that just remembers where the
	// end of the list is (found in MkvsynthOutput)
	for(i = 0; i < output->outputBreadth; i++)
		tmp = tmp->next;
		
	output->outputBreadth++;
	tmp->next = malloc(sizeof(MkvsynthSemaphoreList));
	
	input->remainingBuffer = &tmp->remainingBuffer;
	input->consumedBuffer = &tmp->consumedBuffer;
	
	sem_init(input->remainingBuffer, 10, 0);
	sem_init(input->consumedBuffer, 0, 10);
	
	input->currentFrame = output->recentFrame;
	input->metaData = output->metaData;
	
	return input;
}

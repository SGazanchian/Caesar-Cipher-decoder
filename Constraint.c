//
// Created by Sadra on 11/17/21.
//


#define BUFF 1000

#define FIFO_FINDER_PATH "./FinderPipe"
#define FIFO_FINDER_PATH_MP "FinderPipeMP"

#define FIFO_DECODER_PATH "DecoderPipe"
#define FIFO_PLACER_PATH "PlacerPipe"

#define FIFO_PLACER_PATH_MP "./PlacerPipeMP"


#define EXEC_FINDER "./OS_Finder"
#define EXEC_PLACER "./OS_Placer"
#define EXEC_DECODER "./OS_Decoder"


struct pos{
    int start;
    int end;
};
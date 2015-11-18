template<int size>
class MedianQueue {
  int first = 0;
  int last = 0;
  int length = 0;
  int buffer[size];
public:
  /*MedianQueue() {
    first = 0;
    last = 0;
    length = 0;
  }*/
  
  void Push(int value) {
    if(length == size) {
      Pop();
    }
    buffer[last] = value;
    last = (last+1)%size;
    length++;
    //PrintBuffers();
  }

  int Pop() {
    if(length == 0) return 0;
    int ret = buffer[first];
    first = (first+1)%size;
    length--;
    return ret;
  }

  void PrintBuffers() {
    for( int i = 0; i < size; ++i ) {
      Serial.print(buffer[i]);
      Serial.print("\t");
    }
    Serial.println();
    
  }

  int GetMedian() {
    int tempBuffer[size];
    //PrintBuffers
    memcpy(tempBuffer, buffer, size*sizeof(int));
    qsort(tempBuffer, size, sizeof(int), compare);
    return tempBuffer[(size/2)];
  }
  
  static int compare(const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
  }
  
};


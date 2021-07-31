using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TelemetryModelSatellite.source
{
    public class Chunk
    {
        /* 
          Chunk tempChunk = new Chunk{chunk_count = BAToUInt16(ReceivedData, 0),
                                      frame_number = BAToUInt16(ReceivedData, 2),
                                        chunk_number = ReceivedData[4],
                                        jpegData = ChunkWithoutHeader(ReceivedData, MACROS.HEADER_SIZE, MACROS.CHUNK_SIZE)}();
         */
        public UInt16 chunk_count { get; set; }
        public UInt16 frame_number { get; set; }
        public byte chunk_number { get; set; }

        public byte[] jpegData = new byte[MACROS.CHUNK_SIZE - MACROS.HEADER_SIZE];

    }


    //yeni framenin ilk chunku geldiginde 
    /* 

    Chunk[] temp = new Chunk[temp_chunk.chunk_count];
    temp[temp_chunk.chunk_number] =  temp_chunk;

    bool[] avaible_temp = new bool[temp_chunk.chunk_count];

    Frame tempFrame = new Frame{Chunk= temp,
                                chunk_count = temp_chunk.chunk_count,
                                frame_number = temp_chunk.frame_number,
                                chunk_avaible = avaible_temp
                                is_rendered = false};
     */
    //framenin diger chunklari geldiginde 
    /*
    //new chunk olusturuluyor her iteration da
     */

    class Frame
    {
        public bool is_rendered { get; set; }
        public Chunk[] Chunks { get; set; }


        public int chunk_count { get; set; }

        public int frame_number { get; set; }

        public bool[] chunks_avaible { get; set; }

        public bool is_render_able(bool[] avaible)
        {
            bool tempBool = true;

            foreach (bool x in avaible)
            {
                if (x == false)
                {
                    tempBool = false;
                }
            }
            return tempBool;
        }
        public byte[] GetFrameOrder()
        {
            byte[] temp = new byte[chunk_count * (MACROS.CHUNK_SIZE - MACROS.HEADER_SIZE)];
            for (int i = 0; i < chunk_count; i++)
            {
                for (int j = (Chunks[i].chunk_number * (MACROS.CHUNK_SIZE - MACROS.HEADER_SIZE)), k = 0; j < ((Chunks[i].chunk_number + 1) * (MACROS.CHUNK_SIZE - MACROS.HEADER_SIZE)); j++, k++)
                {
                    temp[j] = Chunks[i].jpegData[k];
                }
            }
            return temp;
        }
    }
}

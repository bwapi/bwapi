using Binarysharp.MemoryManagement;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BWMemoryEdit.BW
{
    public class MemoryItem <T>
    {
        public UInt32 ADDRESS { get; set; }
        public UInt32 SIZE { get; set; }
        public UInt32 COUNT { get; set; }

        public Dictionary<uint, T> items = new Dictionary<uint, T>();

        public UInt32 END { get { return ADDRESS + SIZE * COUNT; } }

        public MemoryItem(UInt32 address, UInt32 size, UInt32 count)
        {
            this.ADDRESS = address;
            this.SIZE = size;
            this.COUNT = count;
        }

        public bool IsValid(Reference<T> reference)
        {
            return reference.value >= ADDRESS && reference.value < END && (reference.value - ADDRESS) % SIZE == 0;
        }
        
        public T GetItem(object referenceObj)
        {
            if (!(referenceObj is Reference<T>)) return default(T);
            Reference<T> reference = (Reference<T>)referenceObj;
            if (!IsValid(reference)) return default(T);

            uint index = (reference.value - ADDRESS) / SIZE;
            return GetItem(index);
        }
        public T GetItem(uint index)
        {
            T result = default(T);
            items.TryGetValue(index, out result);
            return result;
        }
        
        public void InitList(ListBox listBox, MemorySharp sharp)
        {
            for (uint i = 0; i < COUNT; ++i)
            {
                IntPtr mem = new IntPtr(ADDRESS + i * SIZE);
                T thing = (T)Activator.CreateInstance(typeof(T), new object[]{sharp[sharp.MakeRelative(mem)], i});
                items.Add(i, thing);
                listBox.Items.Add(thing);
            }
        }
    }
}

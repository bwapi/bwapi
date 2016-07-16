using BWMemoryEdit.BW;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BWMemoryEdit
{

    /// <summary>
    /// A controller for associating a tab, list, and memory unit with each other.
    /// </summary>
    /// <typeparam name="T">The item that this controller manages (example: Unit, Sprite).</typeparam>
    class ItemController<T>
    {
        Dictionary<object, String> itemCache = new Dictionary<object, string>();
        MemoryItem<T> memItem;
        TabPage tabPage;
        ListBox listBox;

        /// <summary>
        /// Constructor.
        /// </summary>
        /// <param name="memItem">The MemoryItem for validating and retrieving items from references.</param>
        /// <param name="tabPage">The tab page that the list is on.</param>
        /// <param name="list">The list containing all of the MemoryItem values.</param>
        public ItemController(MemoryItem<T> memItem, TabPage tabPage, ListBox list)
        {
            this.memItem = memItem;
            this.tabPage = tabPage;
            this.listBox = list;

            memItem.InitList(list, MainWindow.instance.sharp);
        }

        /// <summary>
        /// Selects a reference in the UI after checking it for validity.
        /// </summary>
        /// <param name="reference">Reference to an item.</param>
        /// <returns>False if the provided reference is not valid in this context. True if it is valid and has been selected.</returns>
        public bool assignReference(object reference)
        {
            object item = memItem.GetItem(reference);
            if (item == null) return false;

            MainWindow.instance.offsetTabs.SelectedTab = tabPage;
            listBox.SelectedItem = item;
            MainWindow.instance.editorGrid.SelectedObject = item;

            return true;
        }

        /// <summary>
        /// Refreshes the item's listbox, updating all items whose names have changed.
        /// </summary>
        public void refresh()
        {
            for (int i = 0; i < listBox.Items.Count; ++i)
            {
                object item = listBox.Items[i];
                String newStr = item.ToString();

                if (!itemCache.ContainsKey(item)) itemCache.Add(item, newStr);
                if (itemCache[item].CompareTo(newStr) != 0)
                {
                    listBox.Items[i] = item;
                }
            }
        }
    }
}

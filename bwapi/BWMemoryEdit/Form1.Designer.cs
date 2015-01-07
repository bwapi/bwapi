namespace BWMemoryEdit
{
    partial class MainWindow
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.offsetList = new System.Windows.Forms.ListBox();
            this.editorGrid = new System.Windows.Forms.PropertyGrid();
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.offsetList);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.editorGrid);
            this.splitContainer1.Size = new System.Drawing.Size(713, 460);
            this.splitContainer1.SplitterDistance = 219;
            this.splitContainer1.TabIndex = 0;
            // 
            // offsetList
            // 
            this.offsetList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.offsetList.FormattingEnabled = true;
            this.offsetList.Location = new System.Drawing.Point(0, 0);
            this.offsetList.Name = "offsetList";
            this.offsetList.Size = new System.Drawing.Size(219, 460);
            this.offsetList.TabIndex = 0;
            this.offsetList.SelectedIndexChanged += new System.EventHandler(this.offsetList_SelectedIndexChanged);
            // 
            // editorGrid
            // 
            this.editorGrid.Dock = System.Windows.Forms.DockStyle.Fill;
            this.editorGrid.HelpVisible = false;
            this.editorGrid.Location = new System.Drawing.Point(0, 0);
            this.editorGrid.Name = "editorGrid";
            this.editorGrid.Size = new System.Drawing.Size(490, 460);
            this.editorGrid.TabIndex = 0;
            // 
            // imageList1
            // 
            this.imageList1.ColorDepth = System.Windows.Forms.ColorDepth.Depth8Bit;
            this.imageList1.ImageSize = new System.Drawing.Size(16, 16);
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(713, 460);
            this.Controls.Add(this.splitContainer1);
            this.Name = "MainWindow";
            this.Text = "BW Memory Editor";
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.ListBox offsetList;
        private System.Windows.Forms.PropertyGrid editorGrid;
        private System.Windows.Forms.ImageList imageList1;
    }
}


namespace MultiplayerTicTacToe
{
    partial class Form1
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
            this.gameControl1 = new MultiplayerTicTacToe.GameControl();
            this.SuspendLayout();
            // 
            // gameControl1
            // 
            this.gameControl1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(33)))), ((int)(((byte)(33)))), ((int)(((byte)(33)))));
            this.gameControl1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.gameControl1.Location = new System.Drawing.Point(0, 0);
            this.gameControl1.MaximumSize = new System.Drawing.Size(350, 450);
            this.gameControl1.MinimumSize = new System.Drawing.Size(350, 450);
            this.gameControl1.Name = "gameControl1";
            this.gameControl1.Size = new System.Drawing.Size(350, 450);
            this.gameControl1.TabIndex = 0;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(350, 449);
            this.Controls.Add(this.gameControl1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Form1";
            this.Text = "LAN X&O";
            this.ResumeLayout(false);

        }

        #endregion

        private GameControl gameControl1;
    }
}


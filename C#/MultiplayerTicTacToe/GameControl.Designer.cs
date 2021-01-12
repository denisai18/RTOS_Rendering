namespace MultiplayerTicTacToe
{
    partial class GameControl
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.boardPnl = new System.Windows.Forms.Panel();
            this.hostBtn = new System.Windows.Forms.Button();
            this.joinBtn = new System.Windows.Forms.Button();
            this.turnLbl = new System.Windows.Forms.Label();
            this.yourLbl = new System.Windows.Forms.Label();
            this.winerLbl = new System.Windows.Forms.Label();
            this.resetBtn = new System.Windows.Forms.Button();
            this.scoreLbl = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.hostingLbl = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // boardPnl
            // 
            this.boardPnl.BackColor = System.Drawing.Color.White;
            this.boardPnl.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.boardPnl.Location = new System.Drawing.Point(19, 118);
            this.boardPnl.Name = "boardPnl";
            this.boardPnl.Size = new System.Drawing.Size(310, 310);
            this.boardPnl.TabIndex = 0;
            // 
            // hostBtn
            // 
            this.hostBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.hostBtn.ForeColor = System.Drawing.Color.White;
            this.hostBtn.Location = new System.Drawing.Point(20, 84);
            this.hostBtn.Name = "hostBtn";
            this.hostBtn.Size = new System.Drawing.Size(87, 28);
            this.hostBtn.TabIndex = 1;
            this.hostBtn.Text = "Host Game";
            this.hostBtn.UseVisualStyleBackColor = true;
            this.hostBtn.Click += new System.EventHandler(this.hostBtn_Click);
            // 
            // joinBtn
            // 
            this.joinBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.joinBtn.ForeColor = System.Drawing.Color.White;
            this.joinBtn.Location = new System.Drawing.Point(241, 84);
            this.joinBtn.Name = "joinBtn";
            this.joinBtn.Size = new System.Drawing.Size(87, 28);
            this.joinBtn.TabIndex = 2;
            this.joinBtn.Text = "Join Game";
            this.joinBtn.UseVisualStyleBackColor = true;
            this.joinBtn.Click += new System.EventHandler(this.joinBtn_Click);
            // 
            // turnLbl
            // 
            this.turnLbl.AutoSize = true;
            this.turnLbl.ForeColor = System.Drawing.Color.White;
            this.turnLbl.Location = new System.Drawing.Point(113, 99);
            this.turnLbl.Name = "turnLbl";
            this.turnLbl.Size = new System.Drawing.Size(35, 13);
            this.turnLbl.TabIndex = 3;
            this.turnLbl.Text = "Turn: ";
            // 
            // yourLbl
            // 
            this.yourLbl.AutoSize = true;
            this.yourLbl.ForeColor = System.Drawing.Color.White;
            this.yourLbl.Location = new System.Drawing.Point(113, 84);
            this.yourLbl.Name = "yourLbl";
            this.yourLbl.Size = new System.Drawing.Size(51, 13);
            this.yourLbl.TabIndex = 4;
            this.yourLbl.Text = "You Are: ";
            // 
            // winerLbl
            // 
            this.winerLbl.AutoSize = true;
            this.winerLbl.Font = new System.Drawing.Font("Microsoft Sans Serif", 20.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.winerLbl.Location = new System.Drawing.Point(74, 30);
            this.winerLbl.Name = "winerLbl";
            this.winerLbl.Size = new System.Drawing.Size(0, 31);
            this.winerLbl.TabIndex = 5;
            // 
            // resetBtn
            // 
            this.resetBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.resetBtn.ForeColor = System.Drawing.Color.White;
            this.resetBtn.Location = new System.Drawing.Point(241, 50);
            this.resetBtn.Name = "resetBtn";
            this.resetBtn.Size = new System.Drawing.Size(88, 28);
            this.resetBtn.TabIndex = 6;
            this.resetBtn.Text = "Reset";
            this.resetBtn.UseVisualStyleBackColor = true;
            this.resetBtn.Visible = false;
            this.resetBtn.Click += new System.EventHandler(this.resetBtn_Click);
            // 
            // scoreLbl
            // 
            this.scoreLbl.AutoSize = true;
            this.scoreLbl.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.scoreLbl.ForeColor = System.Drawing.Color.White;
            this.scoreLbl.Location = new System.Drawing.Point(16, 23);
            this.scoreLbl.Name = "scoreLbl";
            this.scoreLbl.Size = new System.Drawing.Size(42, 40);
            this.scoreLbl.TabIndex = 7;
            this.scoreLbl.Text = "X: 0\r\nO: 0";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.Color.White;
            this.label1.Location = new System.Drawing.Point(16, 3);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(71, 20);
            this.label1.TabIndex = 8;
            this.label1.Text = "SCORE";
            // 
            // hostingLbl
            // 
            this.hostingLbl.AutoSize = true;
            this.hostingLbl.ForeColor = System.Drawing.Color.White;
            this.hostingLbl.Location = new System.Drawing.Point(239, 8);
            this.hostingLbl.Name = "hostingLbl";
            this.hostingLbl.Size = new System.Drawing.Size(90, 13);
            this.hostingLbl.TabIndex = 9;
            this.hostingLbl.Text = "Hosting the game";
            this.hostingLbl.Visible = false;
            // 
            // GameControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(33)))), ((int)(((byte)(33)))), ((int)(((byte)(33)))));
            this.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Controls.Add(this.hostingLbl);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.scoreLbl);
            this.Controls.Add(this.resetBtn);
            this.Controls.Add(this.winerLbl);
            this.Controls.Add(this.yourLbl);
            this.Controls.Add(this.turnLbl);
            this.Controls.Add(this.joinBtn);
            this.Controls.Add(this.hostBtn);
            this.Controls.Add(this.boardPnl);
            this.MaximumSize = new System.Drawing.Size(350, 450);
            this.MinimumSize = new System.Drawing.Size(350, 450);
            this.Name = "GameControl";
            this.Size = new System.Drawing.Size(348, 448);
            this.Load += new System.EventHandler(this.GameControl_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel boardPnl;
        private System.Windows.Forms.Button hostBtn;
        private System.Windows.Forms.Button joinBtn;
        private System.Windows.Forms.Label turnLbl;
        private System.Windows.Forms.Label yourLbl;
        private System.Windows.Forms.Label winerLbl;
        private System.Windows.Forms.Button resetBtn;
        private System.Windows.Forms.Label scoreLbl;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label hostingLbl;
    }
}
